#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"

#include "QueueLL.h"
#include <cmath>

using namespace std;


Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie)
{
   city = new Cities[numCities];
  //city = new City[numCities];
  //evacCity = new bool[numCities];
  //cityDepth = new int[numCities];
  adjList = new int*[numCities];
  totalRoads = 0;
  
  for (int x = 0; x < numCities; x++)
  {
    //deep copy of citie
    city[x].ID = citie[x].ID;
    city[x].x = citie[x].x;
    city[x].y = citie[x].y;
    city[x].population = citie[x].population;
    city[x].evacuees = citie[x].evacuees;
    city[x].roads = citie[x].roads;
    city[x].roadCount = citie[x].roadCount;
    
    //note that all are not evacCity
    city[x].evacCity = false;
    //evacCity[x] = false;
    
    //calculate total number of roads
    totalRoads += city[x].roadCount;
    
    //set up and populate adjList
    adjList[x] = new int[city[x].roadCount + 1];
    
    adjList[x][0] = city[x].ID;
    
    for (int y = 1; y <= city[x].roadCount; y++)
    {
      adjList[x][y] = city[x].roads[y - 1].destinationCityID;
    }
  }
  
  sorted_index = new int[numCities];
  
  print_adjList();
} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  //make note of what cities need to be evacuated
  for (int i = 0; i < numEvacs; i++)
  {
    city[evacIDs[i]].evacCity = true;
  }
  
  //find the center of the cluster
  int rootID = findCenter(evacIDs, numEvacs);
  cout << "root city is: " << rootID << endl;
  
  //breadth first search to calculate depths
  //store depths in array cityDepth
  cout << "depth from root is: ";
  bfs(rootID);
  
  cout << "depths of each city:\n";
  for (int i = 0; i < numCities; i++)
    cout << city[i].depth << " ";
    //cout << cityDepth[i] << " ";
  
  cout << endl;
  
  
  //depth first search to determine which path to take
  dfs(evacRoutes, routeCount);
  
} // evacuate


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


void Evac :: dfs(int *evacIDs, int numEvacs, EvacRoute *evacRoutes, int &routeCount)
{
  /*
    while (there are cities that haven't been completely evacuated)
      for (each city that still has evacuees)
        find a path()
      end for
      
      reset number of people in roads
      update hour
    end while
  */
  // Mark all the vertices as not visited
  bool* visited = new bool[numCities];
  
  for (int i = 0; i < numCities; i++)
    visited[i] = false;
  
  // Call the recursive helper function
  while (!fullyEvacuated(evacIDs, numEvacs))
  {
    for (int j = 0; j < numEvacs; j++)
    {
      if (city[evacIDs[j]].population != 0)  //city still has evacuees
      {
        DFS(city[evacIDs[j]], visited, evacRoutes, routeCount);
      }
    }
    
    //reset number of people in roads
    //update hour
  }
}  // dfs()


int Evac :: DFS(int v, bool visited[], EvacRoute *evacRoutes, int &routeCount)
{
  /*
    Find a path()
    if(current city has been visited)
      return 0;
    else
      mark current city as visited;
      
      if (current city is not a city that has to be evacuated)
        store as much evacuees as possible
      end if
      
      if (there are still evacuees after storing in current city)
        find a path(next city)
      end if
      
    return total flow;
  */
  
  int* path = new int[totalRoads];
  
  if (visited[v])  //current city has been visited
  {
    return 0;
  }
  else  //current city has not been visited
  {
    // Mark the current city as visited
    visited[v] = true;
    
    if (!city[v].evacCity)  //current city does not need to be evacuated
    {
      //store as much evacuees as possible
    }
    
    if (city[v].population != 0)  //still evacuees after storing
    {
      //DFS(next city)
    }
  }
  
  //return total flow;
}  // DFS()


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
}  //findCenter()


void Evac :: bfs(int root_id)
{
  bool new_added = false;//check to make sure if something new was added for depth
  int depth = 0;
  int visited_count = 0;
  int index = 0;
  
  // Mark all the vertices as not visited
  bool *visited = new bool[numCities];
  
  for(int i = 0; i < numCities; i++)
      visited[i] = false;

  // Create a queue for BFS
  QueueLL<int> queue;

  // Mark the current node as visited and enqueue it
  visited[root_id] = true;
  queue.enqueue(root_id);
  //cityDepth[root_id] = depth;
  city[root_id].depth = depth;

  while(!queue.isEmpty())
  {
    if(visited_count == numCities)//if everything was already visited => finished
      break;
      
    // Dequeue a vertex from queue and print it
    root_id = queue.dequeue(); //update index id to traverse down and find the deepest node
    new_added = false;
    
    // cout << root_id << " ";
    sorted_index[index] = root_id;
    index++;

    // Get all adjacent vertices of the dequeued vertexes. 
    // If a adjacent has not been visited, then mark it visited and enqueue it.
    for (int i = 1; i <= city[root_id].roadCount; i++)
    {
      if (!visited[adjList[root_id][i]])
      {
        visited[adjList[root_id][i]] = true;
        queue.enqueue(adjList[root_id][i]);
        //cityDepth[adjList[root_id][i]] = cityDepth[root_id] + 1;
        city[adjList[root_id][i]].depth = city[root_id].depth + 1;
        
        visited_count++;
        new_added = true; //something new was added
      }
    }
    
    //if there are more nodes down and that something new was added
    if((visited_count < numCities) && new_added)
        depth++;
    
  }
  
  cout << depth << endl;
}  // bfs()


void Evac :: print_adjList()
{
  //print adjList
  for (int k = 0; k < numCities; k++)
  {
    for (int l = 0; l <= city[k].roadCount; l++)
    {
      if (l == 0)
      {
        cout << "city ID = " << adjList[k][l] << endl;
        cout << "adjacent to: ";
        continue;
      }
      
      cout << adjList[k][l] << " ";
    }
    cout << endl;
  }
  cout<<endl;
}  //print_adjList()
