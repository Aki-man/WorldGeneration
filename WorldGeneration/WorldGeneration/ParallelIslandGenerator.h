#pragma once
#include "coordinate.h"
#include "IslandGeneratorConfiguration.h"
#include "IslandGenerator.h"
#include "MyHashCompare.h"
#include <map>
#include <string>
#include <iostream>
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_unordered_map.h>
#include "MyHasher.h"
#include "MyEquality.h"

class ParallelIslandGenerator : public IslandGenerator {
	tbb::concurrent_unordered_map<Coordinate, char>* parallelWorldMap;

	/*int startWidth;
	int endWidth;
	int startLength;
	int endLength;
	std::random_device rd;
	IslandGeneratorConfiguration config;*/
public:
	ParallelIslandGenerator(std::map<Coordinate, char>* worldMap, tbb::concurrent_unordered_map<Coordinate, char>* parallelWorldMap,int startWidth, int endWidth, int startLength, int endLength, IslandGeneratorConfiguration config) :
		IslandGenerator(worldMap, startWidth, endWidth, startLength, endLength, config), parallelWorldMap(parallelWorldMap) {
	};
	~ParallelIslandGenerator();
	//std::tuple<int, int> generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength, bool getWider);
	//std::tuple<int, int> generateMountainLengthAndStart(std::tuple<int, int> islandLengthAndStart, int mountainLength, int mountainStart);
	
	virtual  void secondPass() override;
	virtual void generateTileClump(Coordinate coord, int clumpSize, char tile) override;
	void replaceRandomTiles(std::vector<Coordinate> coordinatesToConvert, int clumpSize, char tile);
	virtual void generateRiver(Coordinate startCoord, bool isGoingLeft, bool isGoingUp) override;
	virtual bool isAdjacentTo(Coordinate coord, char tile) override;
	virtual void generateSeaLine(int length) override;
	virtual void generateIslandLine(int island_begin, int island_end, int length) override;
	virtual void generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end) override;

};