#include <iostream>
#include "Map.h"
#include "ParallelMap.h"

int main()
{
    WorldMap map = WorldMap(100, 100);
    map.GenerateFourIslandMap();
   /* map.generateIsland(0, 50, 0, 50);
    map.generateIsland(50, 100, 0, 50);
    map.generateIsland(0, 50, 50, 100);
    map.generateIsland(50, 100, 50, 100);*/
    /*map.generateIsland(0, 50, 0, 100);
    map.generateIsland(50, 100, 0, 50);
    map.generateIsland(50, 100, 50, 100);*/
    

    std::cout << map;
}