// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"

// class coordinates
// {
  // int x;
  // int y;
  // public:
    // void set(int xp, int yp){x = xp; y = yp;}
// };

class Evac
{
  int numCities;
  int* numAdjCities;
  City* c;
  // coordinates* coordCities;
  int** adjList;
  
public:
  Evac(City *cities, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount
  int findCenter(int * evacCitiesID, int numEvacs);
  
}; // class Evac



#endif