// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"
#include "QueueAr.h"

class Cities
{
public:
  int population;
  int evacuees;
  int *roadIDs;
  int roadCount;
  
  bool evacCity;
  Cities():evacuees(0), roadIDs(NULL), roadCount(0), evacCity(false) {}
   ~Cities(){delete [] roadIDs;}
};

class Evac
{
  int numCities;
  int time;
  int roads_used;
  // int totalRoads;

  int* used_roads;
  int *usedIDs;
  
  Cities *city;
  Road *roads;
  
  QueueAr<int>* bfs_queue;
  QueueAr<int>* dfs_queue;
  
  bool* bfs_visited;
  bool* dfs_visited;
  // Road3 *roads;
  
  
  //City3 *evacArray;
  //EvacRoute *evacRoutes2;
  
  void bfs();
  bool space_available(int* evacIDs, int numEvacs);
  int dfs(int cityID, int people_remaining);
  void increment_time(EvacRoute *evacRoutes, int &routeCount);
  bool fullyEvacuated(int *evacIDs, int numEvacs);
  int find_flow(int city_id, int road_id, int total, int needed);
  int max_flow(int cityID);
  
public:
  Evac(City *city, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount

}; // class Evac



#endif
