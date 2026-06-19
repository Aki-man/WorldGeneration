#pragma once
#include "coordinate.h"
#include "IslandGeneratorConfiguration.h"
#include <map>
#include <string>
#include <iostream>
#include <random>

class IslandGenerator {
	std::map<Coordinate, char> *worldMap;
	int startWidth;
	int endWidth;
	int startLength;
	int endLength;
	std::random_device rd;
	IslandGeneratorConfiguration config;
public:
	IslandGenerator(std::map<Coordinate, char>* worldMap, int startWidth, int endWidth, int startLength, int endLength, IslandGeneratorConfiguration config) :
		worldMap(worldMap), startWidth(startWidth), endWidth(endWidth), startLength(startLength), endLength(endLength), config(config), rd(std::random_device()){};
	~IslandGenerator();
	std::tuple<int, int> generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength);
	std::tuple<int, int> generateMountainLengthAndStart(std::tuple<int, int> islandLengthAndStart, int mountainLength, int mountainStart);
	virtual void generateIsland();
	bool shouldIslandGenerate(int length, bool generateIsland, int islandEnd);
	void secondPass();
	void generateTileClump(Coordinate coord, int clumpSize, char tile);
	void generateRiver(Coordinate startCoord, bool isGoingLeft, bool isGoingUp);
	bool isAdjacentTo(Coordinate coord, char tile);
	void generateSeaLine(int length);
	virtual void generateIslandLine(int islandLength, int islandOffset, int length);
	void generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end);

};