#include "evac.h"
#include <cstdlib>
#include "QueueAr.h"
#include "EvacRunner.h"

using namespace std;

Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie), time(1), roads_used(0)
{
  int city_id;
  int road_id;
  city = new Cities[numCities];
  roads = new Road[numRoads * 2];
  used_roads = new int[numRoads * 2];
  used_ids = new int[numRoads];
    
  bfs_queue = new QueueAr<int>(numCities);
  dfs_queue = new QueueAr<int>(numCities);
  bfs_visited = new bool[numCities];
  dfs_visited = new bool[numCities];
  // totalRoads = 0;
    
  for(int x = 0; x < numCities; x++)
  {
    bfs_visited[x] = false;
    dfs_visited[x] = false;
    city_id = citie[x].ID;
    city[city_id].population = citie[x].population;
    city[city_id].roadCount = citie[x].roadCount;
    city[city_id].road_ids = new int[city[city_id].roadCount];
    // totalRoads += city[city_id].roadCount;
    for(int y = 0; y < city[city_id].roadCount; y++)
    {
      road_id = citie[x].roads[y].ID;
      city[city_id].road_ids[y] = road_id;
      roads[road_id].destinationCityID = citie[x].roads[y].destinationCityID;
      roads[road_id].peoplePerHour = citie[x].roads[y].peoplePerHour;
    } 
  }
} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  int city_id;
  routeCount = 0;

  for(int i = 0; i < numEvacs; i++)//init the evacuated city arry
  {
    city_id = evacIDs[i];
    city[city_id].evacCity = true;
    bfs_queue->enqueue(city_id);
    bfs_visited[city_id] = true;
  }

  do
  {
    bfs();
    for(int i = 0; i < numEvacs; i++)//go through each evacuating cities
    {
      city_id = evacIDs[i];
      city[city_id].evacuees += max_flow(city_id);
    } 
    
    for(int i = 0; i < numCities; i++)//reset all the visted ids
      dfs_visited[i] = false;
    
    increment_time(evacRoutes, routeCount);
  }while(fullyEvacuated(evacIDs, numEvacs));

}

int Evac :: max_flow(int city_id)
{
  int max_flow = 0;
  int people_remaining = 0;
  //reset all the visited cities SO FAR
  while(!dfs_queue->isEmpty())
    dfs_visited[dfs_queue->dequeue()]=false;

  //set the current evacuating city is visited
  dfs_queue->enqueue(city_id);
  dfs_visited[city_id] = true;

  for(int i = 0; i < city[city_id].roadCount; i++)//go through each evacuated citiy's road
  {
    people_remaining = city[city_id].population - city[city_id].evacuees;
    if(max_flow >= people_remaining)//make sure number of evacuees doesnt exceed population
      break;
    max_flow = find_flow(city_id, i, max_flow, people_remaining);
  }
  return max_flow;
}

int Evac :: find_flow(int city_id, int road_index, int current_flow, int people_remaining)
{
  int road_id = city[city_id].road_ids[road_index];
  int available_flow = roads[road_id].peoplePerHour - used_roads[road_id];
  int new_flow = 0;
  
  if(available_flow > people_remaining - current_flow)
    available_flow = people_remaining - current_flow;
  
  available_flow = dfs(roads[road_id].destinationCityID, available_flow);
  
  if(available_flow > 0)
  {
    used_ids[roads_used] = road_id;
    roads_used++;
  }
  
  used_roads[road_id] += available_flow;
  new_flow = current_flow + available_flow;
  
  return new_flow;
}

int Evac::dfs(int destination_id, int people_remaining)
{
  int current_flow = 0;
  int available_space = city[destination_id].population - city[destination_id].evacuees;
  if(!dfs_visited[destination_id])// base case, if not visited
  {
    dfs_visited[destination_id] = true; 
    dfs_queue->enqueue(destination_id);
  }
  else
    return 0;
  
  if(city[destination_id].evacCity == false) // not evaccity
  {
    if(people_remaining > available_space)//if no room
    {
      current_flow = available_space;
      city[destination_id].evacuees = city[destination_id].population;
    }
    else 
    {
      city[destination_id].evacuees += people_remaining;
      return people_remaining;
    }
  } // if not an evacuated city then absorb some of the people_remaining

  for(int i = 0; i < city[destination_id].roadCount; i++)
  {
    if(current_flow >= people_remaining)
      break;
     current_flow = find_flow(destination_id, i, current_flow, people_remaining);
  }
  return current_flow;
} 


void Evac::increment_time(EvacRoute *evacRoutes, int &routeCount)
{
  int evac_id;
  for(int i = 0; i < roads_used; i++)// go through accessed roads
  {
    evac_id = used_ids[i];
    evacRoutes[routeCount].roadID = evac_id;
    evacRoutes[routeCount].numPeople = used_roads[evac_id];
    evacRoutes[routeCount].time = time;
    used_roads[evac_id] = 0;
    dfs_visited[roads[evac_id].destinationCityID] = false;
    routeCount++;
  }
  roads_used = 0;
  time++;
}

bool Evac::fullyEvacuated(int* evacIDs, int numEvacs)
{
  for(int i = 0; i < numEvacs; i++)//go through each evacuated city
    if(city[evacIDs[i]].evacuees < city[evacIDs[i]].population)//if space available
      return true;
  return false;
} 
/**
int Evac :: findCenter(int * evacCitiesID, int numEvacs)
{
  //find the center coordinates
  double x = 0.0;
  double y = 0.0;
  
  for(int i = 0; i < numEvacs; i++)
  {
    x += city[evacCitiesID[i]].x;
    y += city[evacCitiesID[i]].y;
  }
  x = x/(double)numEvacs;
  y = y/(double)numEvacs;
  
  //find the closest adjacentID
  int d = 0;
  int min = 9999;
  int the_chosen_one = 0; 
  for(int i = 0; i < numEvacs; i++)
  {
    //distance between center points and coordinates of evacCitiesID
    d = pow( (x - city[evacCitiesID[i]].x), 2 ) - pow( (y - city[evacCitiesID[i]].y), 2);
    d = sqrt(d);
    if(d <= min)
    {
      min = d;
      the_chosen_one = i;
    }
  }
  
  return the_chosen_one;//index closest to centroid
}
**/


void Evac::bfs()
{
  int city_id;
  int destinationID;

  while(!bfs_queue->isEmpty())
  {
    city_id = bfs_queue->dequeue();

    for(int j = 0; j < city[city_id].roadCount;j++)//go through each road of the evac city
    {
      destinationID = roads[city[city_id].road_ids[j]].destinationCityID;
      
      //not yet visited destination
      if(!bfs_visited[destinationID])
      {
        bfs_queue->enqueue(destinationID);
        bfs_visited[destinationID] = true;
      }
  
      if(city[destinationID].evacCity )//if city is evaccity
      {
        city[city_id].roadCount--;//decrease the road count
        city[city_id].road_ids[j] = city[city_id].road_ids[city[city_id].roadCount];//move to end
      }
    }
  } //while queue is not empty

}
