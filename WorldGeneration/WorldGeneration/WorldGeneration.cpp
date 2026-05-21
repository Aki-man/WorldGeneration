#include <iostream>
#include "Map.h"
#include "ParallelMap.h"

int main()
{
    WorldMap map(100, 100);
    map.generateIsland(0, 50, 0, 50);
     map.generateIsland(50, 100, 0, 50);
    map.generateIsland(0, 50, 50, 100);
    map.generateIsland(50, 100, 50, 100);
    /*map.generateIsland(0, 50, 0, 100);
    map.generateIsland(50, 100, 0, 50);
    map.generateIsland(50, 100, 50, 100);*/
    /*for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            map.generateIsland(i * 10, (i + 1) * 10, j * 10, (j + 1) * 10);
        }
    }*/
    std::cout << map;
}