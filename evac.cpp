#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"

#include "QueueLL.h"
#include <cmath>

using namespace std;


Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie)
{
  city = new City[numCities];
  evacCity = new bool[numCities];
  cityDepth = new int[numCities];
  adjList = new int*[numCities];
  
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
    
    //populate evacCity
    evacCity[x] = false;
    
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
    evacCity[evacIDs[i]] = true;
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
    cout << cityDepth[i] << " ";
  
  cout << endl;
  
  
  cout << "sorted index: ";
  for(int i = 0; i < numCities; i++)
    cout << sorted_index[i] << " ";
  cout << endl;
  
  
  //sort adjList from least depth to highest depth
  //cities with same depth should be sorted based on road capacity  (qsort?)
  /*
  sort();
  
  int** temp;
  temp = new int*[numCities];
  
  for(int x = 0; x < numCities; x++)
  {
    temp[x] = new int[city[sorted_index[x]].roadCount];
    temp[x] = adjList[sorted_index[x]];
    //n[x] = city[sorted_index[x]];
  }
  
  adjList = temp;
  
  cout<<"SORTED outcome:"<<endl;
  print_adjList();
  */
  
  
  //depth first search to determine which path to take (network flow)
  
} // evacuate


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
  cityDepth[root_id] = depth;

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
        cityDepth[adjList[root_id][i]] = cityDepth[root_id] + 1;
        
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
