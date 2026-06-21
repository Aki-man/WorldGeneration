#include "Tests.h"
#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"

void Tests::ParallelGenerationTest(int n) {
    std::cout << "Generated island size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serial generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting parallel generation" << std::endl;
    startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    parallelMap.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
}

void Tests::ParallelViewTest(int n) {

    WorldMap map = WorldMap(100, 100);
    map.GenerateFourIslandMap();
    Player serialPlayer(Coordinate(50, 50), map, n);
    std::cout << "Generated view size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serial view" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    serialPlayer.getViewWithShadows();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    std::cout << "------------------------------------------------" << std::endl;
    ParallelWorldMap parallelMap = ParallelWorldMap(100, 100);
    parallelMap.GenerateFourIslandMap();
    ParallelPlayer parallelPlayer(Coordinate(50, 50), map, &parallelMap, n);
    std::cout << "Starting parallel view" << std::endl;
    startTime = tbb::tick_count::now();
    parallelPlayer.getViewWithShadows();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

}

void Tests::SaveWorldToFileTest(int n, std::string fileName) {
    std::cout << "Generated island size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serial generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial generation time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (n <= 200)
        std::cout << map;

    std::cout << "Starting parallel generation" << std::endl;
    startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    parallelMap.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (n <= 200)
        std::cout << parallelMap;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting serial save" << std::endl;
    startTime = tbb::tick_count::now();
    map.save(fileName);
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    map = WorldMap(n, n);
    std::cout << "Starting serial load" << std::endl;
    startTime = tbb::tick_count::now();
    bool success = map.load(fileName);
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (success)
        std::cout << "File loading succeded" << std::endl;
    else
        std::cout << "File loading failed" << std::endl;
    if (n <= 200)
        std::cout << map;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting parallel save" << std::endl;
    startTime = tbb::tick_count::now();
    parallelMap.save(fileName);
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    parallelMap = ParallelWorldMap(n, n);
    std::cout << "Starting serial load" << std::endl;
    startTime = tbb::tick_count::now();
    success = parallelMap.load(fileName);
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (success)
        std::cout << "File loading succeded" << std::endl;
    else
        std::cout << "File loading failed" << std::endl;

    if (n <= 200)
        std::cout << parallelMap;

    std::cout << "------------------------------------------------" << std::endl;

    Player serialPlayer(Coordinate(17, 23), map, n);
    serialPlayer.save(fileName);

    Player newPlayer(Coordinate(0, 0), map, n);
    newPlayer.load(fileName);

    if (serialPlayer.currentCoordinate.x != newPlayer.currentCoordinate.x || serialPlayer.currentCoordinate.y != newPlayer.currentCoordinate.y)
        std::cout << "Player save failed\n";
    else
        std::cout << "Player save succeeded\n";
}

void Tests::BatchTests(int islandSize, int viewSize, std::string fileName) {
    std::cout << "Starting generation tests:" << std::endl;
    ParallelGenerationTest(islandSize);
    std::cout << "Finished generation tests" << std::endl;
    std::cout << "====================================================================" << std::endl;
    std::cout << "Starting view tests:" << std::endl;
    ParallelViewTest(viewSize);
    std::cout << "Finished view tests" << std::endl;
    std::cout << "====================================================================" << std::endl;
    std::cout << "Starting save tests:" << std::endl;
    SaveWorldToFileTest(islandSize, "testFile");
    std::cout << "Finished save tests" << std::endl;
    std::cout << "====================================================================" << std::endl;
}