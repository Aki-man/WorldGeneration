#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"

void PlayerMenu(WorldMap map) {
    Player newPlayer(Coordinate(50, 50), map, 20);
    while (true) {
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button or l to leave" << std::endl;
        std::string entry;
        std::cin >> entry;
        if (entry == "w" || entry == "W")
            newPlayer.moveUp();
        else if (entry == "s" || entry == "S")
            newPlayer.moveDown();
        else if (entry == "a" || entry == "A")
            newPlayer.moveLeft();
        else if (entry == "d" || entry == "D")
            newPlayer.moveRight();
        else if (entry == "l")
            break;
        else
            continue;
        system("cls");
    }
}

/*void ParallelPlayerMenu(ParallelWorldMap map) {
    WorldMap worldmap = WorldMap();
    ParallelPlayer newPlayer(Coordinate(50, 50), worldmap, map, 20);
    while (true) {
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button or l to leave" << std::endl;
        std::string entry;
        std::cin >> entry;
        if (entry == "w" || entry == "W")
            newPlayer.moveUp();
        else if (entry == "s" || entry == "S")
            newPlayer.moveDown();
        else if (entry == "a" || entry == "A")
            newPlayer.moveLeft();
        else if (entry == "d" || entry == "D")
            newPlayer.moveRight();
        else if (entry == "l")
            break;
        else
            continue;
        system("cls");
    }
}*/

void ParallelGenerationTest(int n) {
    std::cout << "Starting serial generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    std::cout << "Starting parallel generation" << std::endl;
    startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    map.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
}

int main()
{
    
    //ParallelWorldMap parallelMap = ParallelWorldMap(1000, 1000);
    //parallelMap.GenerateFourIslandMap();
    //ParallelPlayerMenu(parallelMap);
    ParallelGenerationTest(1000);
}