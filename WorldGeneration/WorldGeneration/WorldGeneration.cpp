#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"
#include "Tests.h"

void PlayerMenu(int n) {
    WorldMap map = WorldMap(n, n);
    map.GenerateFourIslandMap();
   
    Player newPlayer(Coordinate(n/2, n/2), map, 20);
    while (true) {
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button, q to quit, s to save or l to load" << std::endl;
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
            map.save("SerialSave");
            newPlayer.save("SerialSave");
        }
        else if (entry == "l") {
            std::cout << "loading..." << std::endl;
            bool validSave = map.load("SerialSave");
            newPlayer.load("SerialSave");
        }
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
        std::cout << "please press a WASD button, q to quit, s to save or l to load" << std::endl;
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
        else if (entry == "l") {
            std::cout << "loading..." << std::endl;
            bool validSave = map.load("ParallelSave");
            newPlayer.load("ParallelSave");
        }
        else
            continue;
        system("cls");
    }
}


int main()
{
    Tests::ParallelGenerationTest(100);
}