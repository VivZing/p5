// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"


class Evac
{
  int numCities;
  City* city;
  bool* evacCity;
  int* cityDepth;
  
  int** adjList;
  
  int* sorted_index;
  
public:
  Evac(City *cities, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount
  int findCenter(int * evacCitiesID, int numEvacs);
  void bfs(int root_id);
  //void sort();
  void print_adjList();
  
}; // class Evac


#endif
