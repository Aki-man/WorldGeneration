#pragma once
#include "coordinate.h"
#include <map>
#include <string>
#include <iostream>

class WorldMap {
	std::map<Coordinate, char> worldMap;
	int width;
	int length;
public:
	WorldMap() : width(0), length(0), worldMap(std::map<Coordinate, char>()) {};
	WorldMap(int width, int length) : width(width), length(length), worldMap(std::map<Coordinate, char>()) {};
	~WorldMap();
	std::tuple<int, int> generateIslandLengthAndStart(int islandLength, int islandStart, int startWidth, int endWidth, int currentLength);
	virtual void generateIsland(int startWidth, int endWidth, int startLength, int endLength);
	void generateSeaLine(int startWidth, int endWidth, int length);
	virtual void generateIslandLine(int startWidth, int endWidth, int islandLength, int islandOffset, int length);
	void generateCoastLine(int startWidth, int endWidth, int island_begin, int island_end, int length);
	friend std::ostream& operator<<(std::ostream& out, WorldMap& map);
};
