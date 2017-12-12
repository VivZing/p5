#include "evac.h"
#include <cstdlib>
#include "QueueAr.h"
#include "EvacRunner.h"

using namespace std;

Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie), time(1)
{
  int ID, roadID;
  city = new Cities[numCities];
  roads = new Road3[numRoads * 2];
  usedIDs = new int[numRoads * 5];
  
  bfs_queue = new QueueAr<int>(numCities);
  dfs_queue = new QueueAr<int>(numCities);
  bfs_visited = new bool[numCities];
  dfs_visited = new bool[numCities];
    
  for(int i = 0; i < numCitie; i++)
  {
    bfs_visited[i] = false;
    dfs_visited[i] = false;
    ID = citie[i].ID;
    city[ID].evacuees = 0;
    city[ID].population = citie[i].population;
    city[ID].roadCount = citie[i].roadCount;
    city[ID].roadIDs = new int[city[ID].roadCount];
    for(int j = 0; j < city[ID].roadCount; j++)
    {
      city[ID].roadIDs[j] = roadID = citie[i].roads[j].ID;
      roads[roadID].destinationCityID = citie[i].roads[j].destinationCityID;
      roads[roadID].peoplePerHour = citie[i].roads[j].peoplePerHour;
    } // for each road
  }
} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  int ID, total, cityID, people_remaining;
  routeCount = 0;
  evacArray = new City3[numEvacs];
  // visitedCount = 0;

  for(int i = 0; i < numEvacs; i++)//init the evacuated city arry
  {
    ID = evacIDs[i];
    bfs_visited[ID] = true;
    city[ID].end = false;
    city[ID].evacCity = true;
    city[ID].depth = 1;
    bfs_queue->enqueue(ID);
    evacArray[i].ID = ID;
  }

  do//while(remain)
  {
    usedCount = 0;
    bfs();
    //remain = processEvacsArray(evacArray, numEvacs);
    for(int i = 0; i < numEvacs; i++)//go through each evacuating cities
    {
      total = 0;
      cityID = evacArray[i].ID;
      
      //reset all the visited cities SO FAR
      while(!dfs_queue->isEmpty())
        dfs_visited[dfs_queue->dequeue()]=false;//visitedIDs[visitedCount]] = false;
      
      //set the current evacuating city is visited
      dfs_queue->enqueue(cityID);//visitedIDs[visitedCount] = cityID;
      //visitedCount++;
      dfs_visited[cityID] = true;

      for(int i = 0; i < city[cityID].roadCount; i++)//go through each evacuated citiy's road
      {
        people_remaining = city[cityID].population - city[cityID].evacuees;
        if(total >= people_remaining)
          break;
        total = max_flow(cityID, i, total, people_remaining);
      }  // for each road
      city[cityID].evacuees += total;
    } // for each evac city
    
    for(int i = 0; i < numCities; i++)
      dfs_visited[i] = false;
    
    storeRoutes(evacRoutes, routeCount);
    time++;
  }while(processEvacsArray(evacArray, numEvacs));

} // evacuate


int Evac :: max_flow(int city_id, int index, int total, int people_remaining)
{
  int max = total;
  int road_id = city[city_id].roadIDs[index];
  int flow = roads[road_id].peoplePerHour - roads[road_id].used;
  
  if(flow > people_remaining - max)
    flow = people_remaining - max;
  
  flow = dfs(roads[road_id].destinationCityID, flow, city_id);
  roads[road_id].used += flow;
  max += flow;
  
  if(flow > 0)
    usedIDs[usedCount++] = road_id;
  
  return max;
}

void Evac::storeRoutes(EvacRoute *evacRoutes, int &routeCount)
{
  for(int i = 0; i < usedCount; i++)
  {
    evacRoutes[routeCount].roadID = usedIDs[i];
    evacRoutes[routeCount].numPeople = roads[usedIDs[i]].used;
    evacRoutes[routeCount++].time = time;
    roads[usedIDs[i]].used = 0;
    dfs_visited[roads[usedIDs[i]].destinationCityID] = false;
  } // for each road used
}

int Evac::dfs(unsigned short cityID, int people_remaining, unsigned short sourceCityID)
{
  int i, total = 0;
  if(!dfs_visited[cityID])
  {
    dfs_visited[cityID] = true; // stop loops
    dfs_queue->enqueue(cityID);//visitedIDs[visitedCount++] = cityID;
  }
  else
  {
    return 0;
  }
  
  if(!city[cityID].evacCity) // not an evacuated city
  {
    if(people_remaining > city[cityID].population - city[cityID].evacuees)
    {
      total = city[cityID].population - city[cityID].evacuees;
      city[cityID].evacuees = city[cityID].population;
    }
    else // not enough room in city for people_remaining
    {
      city[cityID].evacuees += people_remaining;
      return people_remaining;
    }
  } // if not an evacuated city then absorb some of the people_remaining

  if(city[cityID].end)
    return total;

  for(i = 0; i < city[cityID].roadCount; i++)
  {
    if(total >= people_remaining)
      break;
    //if same city
    if(roads[city[cityID].roadIDs[i]].destinationCityID == sourceCityID)
      continue;  // no infinite loops!
    
     total = max_flow(cityID, i, total, people_remaining);
  }
  return total;
} // dfs()


bool Evac :: fullyEvacuated(int *evacIDs, int numEvacs)
{
  bool fullyEvacuated;
  
  for (int i = 0; i < numEvacs; i++)
  {
    if (city[evacIDs[i]].population == 0)
    {
      fullyEvacuated = true;
    }
    else
    {
      fullyEvacuated = false;
      break;
    }
  }
  
  return fullyEvacuated;
}  //fullyEvacuated()

bool Evac::processEvacsArray(City3 *evacArray, int numEvacs)
{
  // int capacity;
  bool remain = false;
  // int road_ID;
  int city_ID;
  for(int i = 0; i < numEvacs; i++)//go through each evacuated city
  {
    city_ID = evacArray[i].ID;
    if(city[city_ID].evacuees < city[city_ID].population)
    {
      //remain = true;
      return true;
      // capacity = 0;
      // for(int j = 0; j < city[city_ID].roadCount; j++)
      // {
        // road_ID = city[city_ID].roadIDs[j];
        // if(roads[road_ID].peoplePerHour <
          // city[roads[road_ID].destinationCityID].population)
          // capacity += roads[road_ID].peoplePerHour;
        // else
          // capacity += city[roads[road_ID].destinationCityID].population;
      // evacArray[i].ratio = (city[city_ID].population - city[city_ID].evacuees)
          // / (1.0 * capacity);
      // }
    } // if some evacuees still remain
  } // for each city still in the evacArray

  // if(remain)
    // qsort(evacArray, numEvacs, sizeof(City3), City3Cmp);

  return remain;
} // processEvacsArray


void Evac::bfs()//BFS
{
  int ID, destCityID;
  
  int i = bfs_queue->front;
  while(i < bfs_queue->back)//set evacuated city' end as FALSE
  {
    city[bfs_queue->get(i)].end = false;
    i++;
  }

  while(!bfs_queue->isEmpty())//while queue is not empty
  {
    ID = bfs_queue->dequeue();//queue[front];
    //front++;

    for(int j = 0; j < city[ID].roadCount;j++)//go through each road of the evac city
    {
      //roads is an array of ALL the roads
      //one of the evacuated city's road's DESTINATIONCITYID
      destCityID = roads[city[ID].roadIDs[j]].destinationCityID;
      
      //if the destination city is NOT visited
      if(bfs_visited[destCityID] == false)//if visited
      {
        bfs_queue->enqueue(destCityID);//[back] = destCityID;
        //back++;
        city[ID].depth = time + 1;
        bfs_visited[destCityID] = true;//defalut check is false
      }
  
      //if is the evac city
      //one road move == one hour time
      //IF current time and depth allows it
      if(city[destCityID].evacCity && city[destCityID].depth < time - 1)//not root and depth can be further
      // more than one level back in tree so eliminate road
      {
        //move current road to the last block
        //essentially deleting the current road
        //int temp = city[ID].roadIDs[j];
        city[ID].roadCount--;//decrease the road count
        city[ID].roadIDs[j] = city[ID].roadIDs[city[ID].roadCount];//set the current road as the last one
        //city[ID].roadIDs[city[ID].roadCount] = temp;
      }
    } // for each road
  } // while more in the queue for this round

} // processQ()
