#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"

using namespace std;

Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie)
{
  //cout << "numCitie = " << numCitie << endl;
  //cout << "numRoads = " << numRoads << endl;
  
  numAdjCities = new int[numCities];
  adjList = new int*[numCities];
  
  for (int i = 0; i < numCities; i++)
  {
    //cout << "city ID = " << citie[i].ID << endl;
    //cout << "roadCount = " << citie[i].roadCount << endl << endl;
    
    numAdjCities[i] = citie[i].roadCount;
    adjList[i] = new int[citie[i].roadCount + 1];
    
    //populate adjList
    for (int j = 1; j <= numAdjCities[i]; j++)
    {
      adjList[i][j] = citie[i].roads[j - 1].destinationCityID;
    }
    
    adjList[i][0] = citie[i].ID;
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
  //depth first search to determine which path to take (network flow)
} // evacuate

