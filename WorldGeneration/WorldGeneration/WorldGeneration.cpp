#include <iostream>
#include "Map.h"
#include "ParallelMap.h"
#include "Player.h"

int main()
{
    WorldMap map = WorldMap(1000, 1000);
    map.GenerateFourIslandMap();
    //std::cout << map;

    Player newPlayer(Coordinate(50, 50), map, 20);
    while (true) {
        newPlayer.getViewWithShadows();
        std::cout << newPlayer << std::endl;
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