#pragma once
#include "coordinate.h"
#include "IslandGeneratorConfiguration.h"
#include <map>
#include <string>
#include <iostream>
#include <random>

class IslandGenerator {
	std::map<Coordinate, char> *worldMap;
protected:
	
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
	//std::tuple<int, int> generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength, bool getWider);
	//std::tuple<int, int> generateMountainLengthAndStart(std::tuple<int, int> islandLengthAndStart, int mountainLength, int mountainStart);
	void generateIsland();
	void generateIslandOrIslandWithMountain(int i, int islandLength, int islandStart, int mountainLength, int mountainStart, bool generateIsland, bool generateMountain, bool mountainEndOfGeneration);
	bool shouldIslandGenerate(int length, bool generateIsland, int islandEnd);
	virtual void secondPass();
	virtual void generateTileClump(Coordinate coord, int clumpSize, char tile);
	virtual void generateRiver(Coordinate startCoord, bool isGoingLeft, bool isGoingUp);
	virtual bool isAdjacentTo(Coordinate coord, char tile);
	virtual void generateSeaLine(int length);
	virtual void generateIslandLine(int islandLength, int islandOffset, int length);
	virtual void generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end);

};