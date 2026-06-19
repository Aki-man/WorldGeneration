#pragma once
#include "coordinate.h"
#include "IslandGeneratorConfiguration.h"
#include <map>
#include <string>
#include <iostream>

class IslandGenerator {
	std::map<Coordinate, char> *worldMap;
	int startWidth;
	int endWidth;
	int startLength;
	int endLength;
	IslandGeneratorConfiguration config;
public:
	IslandGenerator(std::map<Coordinate, char>* worldMap, int startWidth, int endWidth, int startLength, int endLength, IslandGeneratorConfiguration config) :
		worldMap(worldMap), startWidth(startWidth), endWidth(endWidth), startLength(startLength), endLength(endLength), config(config){};
	~IslandGenerator();
	std::tuple<int, int> generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength);
	virtual void generateIsland();
	void secondPass();
	bool isAdjacentTo(Coordinate coord, char tile);
	void generateSeaLine(int length);
	virtual void generateIslandLine(int islandLength, int islandOffset, int length);
	void generateCoastLine(int island_begin, int island_end, int length);

};