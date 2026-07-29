#pragma once
#include "coordinate.h"
#include "IslandGeneratorConfiguration.h"
#include "HashInjection.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <random>

class IslandGenerator {
	std::unordered_map<Coordinate, char> *worldMap;
protected:
	
	int startWidth;
	int endWidth;
	int startLength;
	int endLength;
	std::random_device rd;
	IslandGeneratorConfiguration config;
public:
	IslandGenerator(std::unordered_map<Coordinate, char>* worldMap, int startWidth, int endWidth, int startLength, int endLength, IslandGeneratorConfiguration config) :
		worldMap(worldMap), startWidth(startWidth), endWidth(endWidth), startLength(startLength), endLength(endLength), config(config), rd(std::random_device()){};
	~IslandGenerator();
	void generateIsland();
	void generateIslandOrIslandWithMountain(int i, int islandLength, int islandStart, int mountainLength, int mountainStart, bool generateIsland, bool generateMountain, bool mountainEndOfGeneration);
	bool shouldIslandGenerate(int length, bool generateIsland, int islandEnd);
	virtual void insert(Coordinate coord, char tile);
	virtual void replace(Coordinate coord, char tile);
	virtual char get(Coordinate coord);
	virtual bool contains(Coordinate coord);
	void secondPass();
	void generateTileClump(Coordinate coord, int clumpSize, char tile);
	void generateRiver(Coordinate startCoord, bool isGoingLeft, bool isGoingUp);
	bool isAdjacentTo(Coordinate coord, char tile);
	void generateSeaLine(int length);
	void generateIslandLine(int islandLength, int islandOffset, int length);
	void generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end);

};