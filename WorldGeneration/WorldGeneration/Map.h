#pragma once
#include "coordinate.h"
#include <map>
#include <string>
#include <iostream>

class WorldMap {
protected:
	
	int width;
	int length;
public:
	std::map<Coordinate, char> worldMap;
	WorldMap() : width(0), length(0), worldMap(std::map<Coordinate, char>()) {};
	WorldMap(int width, int length) : width(width), length(length), worldMap(std::map<Coordinate, char>()) {};
	~WorldMap();
	virtual void GenerateFourIslandMap();
	virtual void print(std::ostream& out);
	friend std::ostream& operator<<(std::ostream& out, WorldMap& map);
	
};
