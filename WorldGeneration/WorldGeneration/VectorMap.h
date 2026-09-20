#pragma once
#include "Map.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_hash_map.h>
class VectorMap : public WorldMap{
public:
	std::vector<char> vectorWorldMap;
	VectorMap() : WorldMap(), vectorWorldMap(std::vector<char>()) {};
	VectorMap(int width, int length) : WorldMap(width, length), vectorWorldMap(std::vector<char>(width*length)) {};
	~VectorMap();
	char get(Coordinate coord) override;
	void insert(Coordinate coord, char tile) override;
	bool contains(Coordinate coord) override;
	void GenerateOneIslandMap() override;
	void GenerateFourIslandMap() override;
};