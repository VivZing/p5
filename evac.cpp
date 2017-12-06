#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"
#include <cmath>

using namespace std;

Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie)
{
  //cout << "numCitie = " << numCitie << endl;
  //cout << "numRoads = " << numRoads << endl;
  
  //deep copy city TEMP
  c = new City[numCities];//citie;
  for(int x = 0; x < numCitie; x++)
  {
    c[x].ID = citie[x].ID;
    c[x].x = citie[x].x;
    c[x].y = citie[x].y;
    c[x].population = citie[x].population;
    c[x].evacuees = citie[x].evacuees;
    c[x].roads = citie[x].roads;
    c[x].roadCount = citie[x].roadCount;
  }
  
  //init the adjacent list
  numAdjCities = new int[numCities];
  // coordCities = new coordinates[numCities];
  adjList = new int*[numCities];
  
  for (int i = 0; i < numCities; i++)
  {
    //cout << "city ID = " << citie[i].ID << endl;
    //cout << "roadCount = " << citie[i].roadCount << endl << endl;
    
    numAdjCities[i] = citie[i].roadCount;//roadcount is the number of roads connecting to adj cities
    // coordCities[i].set(citie[i].x, citie[i].y);
    adjList[i] = new int[citie[i].roadCount + 1];
    
    //populate adjList
    adjList[i][0] = citie[i].ID;
    
    for (int j = 1; j <= numAdjCities[i]; j++)
    {
      adjList[i][j] = citie[i].roads[j - 1].destinationCityID;
    }
    
  }
  
  
  //print adjList
  for (int k = 0; k < numCities; k++)
  {
    for (int l = 0; l <= numAdjCities[k]; l++)
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
  
  
  //find the center of the cluster
  //breadth first search to calculate depths
  //store depths somewhere (array?) where the higher depths are at the front)
  
} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  int rootID = findCenter(evacIDs, numEvacs);
  cout << "root is: " << rootID << endl;
  //depth first search to determine which path to take (network flow)
} // evacuate


int Evac :: findCenter(int * evacCitiesID, int numEvacs)
{
  //find the center coordinates
  double x = 0.0;
  double y = 0.0;
  for(int i = 0; i < numEvacs; i++)
  {
    x += c[evacCitiesID[i]].x;
    y += c[evacCitiesID[i]].y;
  }
  x = x/(double)numEvacs;
  y = y/(double)numEvacs;
  
  cout << "found centroid" << endl;
  //find the closest adjacentID
  int d = 0;
  int min = 9999;
  int the_chosen_one = 0; 
  for(int i = 0; i < numEvacs; i++)
  {
    //distance between center points and coordinates of evacCitiesID
    d = pow( (x - c[evacCitiesID[i]].x), 2 ) - pow( (y - c[evacCitiesID[i]].y), 2);
    d = sqrt(d);
    if(d <= min)
    {
      min = d;
      the_chosen_one = i;
    }
  }
  return the_chosen_one;//index closest to centroid
}
