#include <iostream>
#include "Map.h"
#include "ParallelMap.h"
#include "Player.h"

int main()
{
    WorldMap map = WorldMap(100, 100);
    map.GenerateFourIslandMap();
    std::cout << map;

    Player newPlayer(Coordinate(20, 20), map, 10);
    while (true) {
        newPlayer.getView();

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
        else
            break;
    }
    

    
}