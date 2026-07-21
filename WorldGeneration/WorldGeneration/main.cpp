#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"
#include "Tests.h"
#include "SaveSystemHelper.h"

void PlayerMenu(WorldMap& map, Player& newPlayer) {
    /*WorldMap map = WorldMap(n, n);
    if (load) {
        map.load("SerialSave");
        if (map.worldMap.size() == 0)
            return;
    }
    else if (fourIsland)
        map.GenerateFourIslandMap();
    else
        map.GenerateOneIslandMap();*/
   
    //Player newPlayer(Coordinate(n/2, n/2), map, 20);
    /*if (load)
        newPlayer.load("SerialSave");*/
    while (true) {
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button, q to quit, save to save" << std::endl;
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
        else if (entry == "q")
            break;
        else if (entry == "save") {
            std::cout << "saving..." << std::endl;
            map.parallelSave("SerialSave");
            newPlayer.save("SerialSave");
        }
        else
            continue;
        system("cls");
    }
}

void ParallelPlayerMenu(ParallelWorldMap map, ParallelPlayer newPlayer) {
   /*ParallelWorldMap map = ParallelWorldMap(n, n);
    if (load) {
        map.load("ParallelSave");
        if (map.parallelWorldMap.size() == 0) {
            return;
        }
            
    }
    else if (fourIsland)
        map.GenerateFourIslandMap();
    else
        map.GenerateOneIslandMapParallel();

    WorldMap worldmap = WorldMap();
    ParallelPlayer newPlayer(Coordinate(n/2, n/2), worldmap, &map, 20);
    if(load)
        newPlayer.load("ParallelSave");*/
    while (true) {
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button, q to quit, save to save" << std::endl;
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
        else if (entry == "q")
            break;
        else if (entry == "save") {
            std::cout << "saving..." << std::endl;
            map.save("ParallelSave");
            newPlayer.save("ParallelSave");
        }
        else
            continue;
        system("cls");
    }
}

void generateNewIslandMenu() {
   
    while (true) {
        system("cls");
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Generate a serial four island map" << std::endl;
        std::cout << "2: Generate a serial one island map" << std::endl;
        std::cout << "3: Generate a parallel four island map" << std::endl;
        std::cout << "4: Generate a parallel one island map" << std::endl;

        std::string userInput = "";
        std::cin >> userInput;

        if (userInput == "q") {
            break;
        }

        std::cout << "Please pick a size for the generated map:" << std::endl;
        std::string sizeInput;
        std::cin >> sizeInput;
        int size = 0;
        try {
            size = std::stoi(sizeInput);
        }
        catch (const std::invalid_argument) {
            std::cout << "Invalid size parameter." << std::endl;
            std::cin >> sizeInput;
            if(userInput != "q")
                continue;
        }

        if (size < 50)
            size = 50;

        //WorldMap map = WorldMap(size, size);
       // Player newPlayer(Coordinate(size / 2, size / 2), map, 20);

        if (userInput == "1") {
            WorldMap map = WorldMap(size, size);
            Player newPlayer(Coordinate(size / 2, size / 2), map, 20);
            map.GenerateFourIslandMap();
            PlayerMenu(map, newPlayer);
        }
        else if (userInput == "2") {
            WorldMap map = WorldMap(size, size);
            Player newPlayer(Coordinate(size / 2, size / 2), map, 20);
            map.GenerateOneIslandMap();
            PlayerMenu(map, newPlayer);
        }
        if (userInput == "3") {
            ParallelWorldMap map = ParallelWorldMap(size, size);
            WorldMap smap = WorldMap(size, size);
            ParallelPlayer newPlayer(Coordinate(size / 2, size / 2), smap, &map, 20);
            map.GenerateFourIslandMap();
            ParallelPlayerMenu(map, newPlayer);
        }
        else if (userInput == "4") {
            ParallelWorldMap map = ParallelWorldMap(size, size);
            WorldMap smap = WorldMap(size, size);
            ParallelPlayer newPlayer(Coordinate(size / 2, size / 2), smap, &map, 20);
            map.GenerateOneIslandMap();
            ParallelPlayerMenu(map, newPlayer);
        }
        
    }
}

void loadIslandMenu() {
   
    while (true) {
        system("cls");
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Load a serial map" << std::endl;
        std::cout << "2: Load a parallel map" << std::endl;
        std::string userInput = "";

        std::cin >> userInput;

        if (userInput == "1") {
            WorldMap map;
            map.load("SerialSave");
            Player newPlayer = Player(Coordinate(0, 0), map, 20);
            newPlayer.load("SerialSave");
            PlayerMenu(map, newPlayer);
        }
        else if (userInput == "2") {
            ParallelWorldMap map;
            map.load("ParallelSave");
            WorldMap smap;
            ParallelPlayer newPlayer(Coordinate(0, 0), smap, &map, 20);
            newPlayer.load("ParallelSave");
            ParallelPlayerMenu(map, newPlayer);
        }
        else if (userInput == "q") {
            break;
        }
    }

}

void testsMenu() {
    
    while (true) {
        system("cls");
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Run generation tests" << std::endl;
        std::cout << "2: Run view tests" << std::endl;
        std::cout << "3: Run save tests" << std::endl;
        std::cout << "4: Run one island generation tests" << std::endl;
        std::cout << "5: Run all tests" << std::endl;
        
        std::string userInput = "";
        std::cin >> userInput;

        if (userInput == "1") {
            int sizeInput = 0;
            std::cout << "Input island size for generation test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::ParallelGenerationTest(sizeInput);
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "2") {
            int sizeInput = 0;
            std::cout << "Input view size for view test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::ParallelViewTest(sizeInput);
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "3") {
            int sizeInput = 0;
            std::cout << "Input island size for save test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::SaveWorldToFileTest(sizeInput, "testFile");
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "4") {
            int sizeInput = 0;
            std::cout << "Input island size for generation test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::ParallelOneIslandGenerationTest(sizeInput);
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "5") {
            int islandSizeInput = 0;
            std::cout << "Input island size for batch test:" << std::endl;
            std::cin >> islandSizeInput;

            int viewSizeInput = 0;
            std::cout << "Input view size for batch test:" << std::endl;
            std::cin >> viewSizeInput;

            system("cls");
            Tests::BatchTests(islandSizeInput, viewSizeInput, "testFile");
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "q") {
            break;
        }
    }
}

void mainMenu() {
   

    
    while (true) {
        system("cls");
        std::cout << "Welcome to the random world generation app!" << std::endl;
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Generate a new island and start playing" << std::endl;
        std::cout << "2: Load an already existing world" << std::endl;
        std::cout << "3: Run some tests" << std::endl;

        std::string userInput = "";
        std::cin >> userInput;

        if (userInput == "1") {
            generateNewIslandMenu();
        }
        else if (userInput == "2") {
            loadIslandMenu();
        }
        else if (userInput == "3") {
            testsMenu();
        }
        else if (userInput == "q") {
            break;
        }
       
    }
    
    
}

int main()
{
    SaveSystemHelper::CheckSaveDirectory();
    mainMenu();
}