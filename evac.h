// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"
#include <iostream>  //for NULL

using namespace std;

class Cities
{
public:
  int ID;
  int x;
  int y;
  int population;
  int evacuees;
  Road *roads;
  int roadCount;
  
  bool evacCity;
  int depth;


  Cities() : evacuees(0), roads(NULL), roadCount(0){}
  ~Cities(){delete [] roads;}
};  // class Cities


class Evac
{
  int numCities;
  Cities* city;
  //City* city;
  //bool* evacCity;
  //int* cityDepth;
  int totalRoads;
  
  int** adjList;
  
  int* path;
  
  int* sorted_index;
  
public:
  Evac(City *cities, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount
  bool fullyEvacuated(int *evacIDs, int numEvacs);
  void dfs(int *evacIDs, int numEvacs, EvacRoute **evacRoutes, int &routeCount);
  int DFS(int v, bool visited[], EvacRoute **evacRoutes, int &routeCount);
  int findCenter(int * evacCitiesID, int numEvacs);
  void bfs(int root_id);
  void print_adjList();
  
}; // class Evac


#endif
