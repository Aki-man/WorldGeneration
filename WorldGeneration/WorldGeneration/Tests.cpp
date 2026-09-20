#include "Tests.h"
#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"
#include "VectorMap.h"
#include "ArrayMap.h"

void Tests::ParallelGenerationTest(int n) {

    std::cout << "Generated island size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting parallel generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    parallelMap.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << parallelMap << std::endl;

    
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serial generation" << std::endl;
    startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << map << std::endl;
    
}

void Tests::VectorGenerationTest(int n) {

    std::cout << "Generated island size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting parallel generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    parallelMap.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << parallelMap << std::endl;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting array generation" << std::endl;
    startTime = tbb::tick_count::now();
    ArrayMap arrayMap = ArrayMap(n, n);
    arrayMap.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Array time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << arrayMap << std::endl;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting vector generation" << std::endl;
    startTime = tbb::tick_count::now();
    VectorMap vectorMap = VectorMap(n, n);
    vectorMap.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Vector time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << vectorMap << std::endl;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serial generation" << std::endl;
    startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << map << std::endl;

}

void Tests::ParallelOneIslandGenerationTest(int n) {

    std::cout << "Generated island size " << n << "*" << n << std::endl;
    

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting parallel generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    ParallelWorldMap trueParallelMap = ParallelWorldMap(n, n);
    trueParallelMap.GenerateOneIslandMapParallel();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << trueParallelMap << std::endl;



    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serial generation" << std::endl;
    startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateOneIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << map << std::endl;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Starting serialized parallel generation" << std::endl;
     startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    parallelMap.GenerateOneIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    if (n <= 200)
        std::cout << parallelMap << std::endl;


}

void Tests::ParallelViewTest(int n) {

    std::cout << "Setting up..." << std::endl;
    WorldMap map = WorldMap(200, 200);
    map.GenerateFourIslandMap();
    Player serialPlayer(Coordinate(50, 50), map, n);

    ParallelWorldMap parallelMap = ParallelWorldMap(200, 200);
    parallelMap.GenerateFourIslandMap();
    ParallelPlayer parallelPlayer(Coordinate(50, 50), parallelMap, n);

    std::cout << "Generated view size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting parallel view" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    parallelPlayer.getViewWithShadows();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting serial view" << std::endl;
    startTime = tbb::tick_count::now();
    serialPlayer.getViewWithShadows();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    

}

void Tests::SaveWorldToFileTest(int n, std::string fileName) {

    std::cout << "Generated island size " << n << "*" << n << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting parallel generation" << std::endl;
    tbb::tick_count startTime = tbb::tick_count::now();
    ParallelWorldMap parallelMap = ParallelWorldMap(n, n);
    parallelMap.GenerateFourIslandMap();
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel generation time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (n <= 200)
        std::cout << parallelMap;

    std::cout << "Starting parallel save" << std::endl;
    startTime = tbb::tick_count::now();
    parallelMap.save(fileName + "ParallelSave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    parallelMap = ParallelWorldMap(n, n);
    std::cout << "Starting parallel load" << std::endl;
    startTime = tbb::tick_count::now();
    bool success = parallelMap.load(fileName + "ParallelSave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Parallel loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (success)
        std::cout << "File loading succeded" << std::endl;
    else
        std::cout << "File loading failed" << std::endl;

    if (n <= 200)
        std::cout << parallelMap;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting vector generation" << std::endl;
    startTime = tbb::tick_count::now();
    VectorMap vectorMap = VectorMap(n, n);
    vectorMap.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Vector generation time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (n <= 200)
        std::cout << vectorMap;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting vector save" << std::endl;
    startTime = tbb::tick_count::now();
    vectorMap.save(fileName + "VectorSave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Vector saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    vectorMap = VectorMap(n, n);
    std::cout << "Starting vector load" << std::endl;
    startTime = tbb::tick_count::now();
    success = vectorMap.load(fileName + "VectorSave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Vector loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (success)
        std::cout << "File loading succeded" << std::endl;
    else
        std::cout << "File loading failed" << std::endl;
    if (n <= 200)
        std::cout << vectorMap;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting array generation" << std::endl;
    startTime = tbb::tick_count::now();
    ArrayMap arrayMap = ArrayMap(n, n);
    arrayMap.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Array generation time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (n <= 200)
        std::cout << arrayMap;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting array save" << std::endl;
    startTime = tbb::tick_count::now();
    arrayMap.save(fileName + "ArraySave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Array saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    arrayMap = ArrayMap(n, n);
    std::cout << "Starting array load" << std::endl;
    startTime = tbb::tick_count::now();
    success = arrayMap.load(fileName + "ArraySave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Array loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (success)
        std::cout << "File loading succeded" << std::endl;
    else
        std::cout << "File loading failed" << std::endl;
    if (n <= 200)
        std::cout << arrayMap;

    std::cout << "------------------------------------------------" << std::endl;


    std::cout << "Starting serial generation" << std::endl;
    startTime = tbb::tick_count::now();
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial generation time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (n <= 200)
        std::cout << map;

    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Starting serial save" << std::endl;
    startTime = tbb::tick_count::now();
    map.save(fileName + "SerialSave");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    map = WorldMap(n, n);
    std::cout << "Starting serial load" << std::endl;
    startTime = tbb::tick_count::now();
    success = map.load(fileName + "SerialSave");
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

    Player serialPlayer(Coordinate(17, 23), map, n);
    serialPlayer.save(fileName + "PlayerSave");

    Player newPlayer(Coordinate(0, 0), map, n);
    newPlayer.load(fileName + "PlayerSave");

    if (serialPlayer.currentCoordinate.x != newPlayer.currentCoordinate.x || serialPlayer.currentCoordinate.y != newPlayer.currentCoordinate.y)
        std::cout << "Player save failed\n";
    else
        std::cout << "Player save succeeded\n";
}

void Tests::SaveWorldToFileTestSerial(int n, std::string fileName)
{
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

    std::cout << "Starting serial parallel save" << std::endl;
    startTime = tbb::tick_count::now();
    map.parallelSave(fileName + "Parallel");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial parallel saving time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    map = WorldMap(n, n);
    std::cout << "Starting parallel serial load" << std::endl;
    startTime = tbb::tick_count::now();
    success = map.load(fileName + "Parallel");
    endTime = tbb::tick_count::now();
    std::cout << "done\n";
    std::cout << "Serial parallel loading time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";

    if (success)
        std::cout << "File loading succeded" << std::endl;
    else
        std::cout << "File loading failed" << std::endl;
    if (n <= 200)
        std::cout << map;

    std::cout << "------------------------------------------------" << std::endl;
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