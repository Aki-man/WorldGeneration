#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"

void PlayerMenu(int n) {
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
   
    Player newPlayer(Coordinate(n/2, n/2), map, 20);
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

void ParallelPlayerMenu(int n) {
    ParallelWorldMap map = ParallelWorldMap(n, n);
    map.GenerateFourIslandMap();

    WorldMap worldmap = WorldMap();
    ParallelPlayer newPlayer(Coordinate(n/2, n/2), worldmap, &map, 20);
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

void ParallelViewTest(int n) {
    
    WorldMap map = WorldMap(100, 100);
    map.GenerateFourIslandMap();
    Player serialPlayer(Coordinate(50, 50), map, n);
    std::cout << "Starting serial view" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    serialPlayer.getViewWithShadows();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    ParallelWorldMap parallelMap = ParallelWorldMap(100, 100);
    map.GenerateFourIslandMap();
    ParallelPlayer parallelPlayer(Coordinate(50, 50), map, &parallelMap, n);
    std::cout << "Starting parallel view" << std::endl;
    startTime = tbb::tick_count::now();
    parallelPlayer.getViewWithShadows();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

}

void SaveWorldToFile(int n, std::string fileName) {
    std::cout << "Starting serial generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial generation time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    std::cout << map;

    std::cout << "Starting serial save" << std::endl;
    startTime = tbb::tick_count::now();
    map.save(fileName);
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    std::cout << "Starting serial load" << std::endl;
    startTime = tbb::tick_count::now();
    map.load(fileName);
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    std::cout << map;
}

int main()
{
    SaveWorldToFile(100, "testSave");
}