// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"


class Evac
{
  int numCities;
  int* numAdjCities;
  int** adjList;
  
public:
  Evac(City *cities, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount
  
}; // class Evac



#endif
