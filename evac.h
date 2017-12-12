// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"
#include "QueueAr.h"

class Cities
{
public:
  bool end;
  bool evacCity;//if evac city or nah
  
  int *roadIDs;
  int population;
  int evacuees;
  int roadCount;
  int depth;
  Cities():end(true), evacCity(false), evacuees(0)
  {}
}; // Cities class


class City3
{
public:
  unsigned short ID;
  float ratio;
}; // City3 class


class Road3
{
public:
  unsigned short destinationCityID;
  int peoplePerHour;
  int used;
  Road3():used(0) {}
}; // Road3 class

class Evac
{
  QueueAr<int>* bfs_queue;
  QueueAr<int>* dfs_queue;
  bool* bfs_visited;
  bool* dfs_visited;
  
  Cities *city;
  Road3 *roads;
  City3 *evacArray;
  EvacRoute *evacRoutes2;
  int *usedIDs;

      
  int numCities;
  int time;
  int usedCount;

  void bfs();
  bool processEvacsArray(City3 *evacArray, int numEvacs);
  int dfs(unsigned short cityID, int needed, unsigned short sourceCityID);
  void storeRoutes(EvacRoute *evacRoutes, int &routeCount);
  bool fullyEvacuated(int *evacIDs, int numEvacs);
  int max_flow(int city_id, int index, int total, int needed);
  
public:
  Evac(City *city, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount

}; // class Evac



#endif
