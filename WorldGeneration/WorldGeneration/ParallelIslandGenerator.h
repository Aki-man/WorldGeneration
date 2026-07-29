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
#include <tbb/concurrent_hash_map.h>

class ParallelIslandGenerator : public IslandGenerator {
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>* parallelWorldMap;
public:
	ParallelIslandGenerator(std::unordered_map<Coordinate, char>* worldMap, tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>* parallelWorldMap,int startWidth, int endWidth, int startLength, int endLength, IslandGeneratorConfiguration config) :
		IslandGenerator(worldMap, startWidth, endWidth, startLength, endLength, config), parallelWorldMap(parallelWorldMap) {
	};
	~ParallelIslandGenerator();
	virtual void insert(Coordinate coord, char tile) override;
	virtual void replace(Coordinate coord, char tile) override;
	virtual char get(Coordinate coord) override;
	virtual bool contains(Coordinate coord) override;
	void parallelSecondPass();
	

};