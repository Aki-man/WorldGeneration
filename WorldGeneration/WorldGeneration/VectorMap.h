#pragma once
#include "Map.h"
class VectorMap : public WorldMap{
public:
	std::vector<char> vectorWorldMap;
	VectorMap() : WorldMap(), vectorWorldMap(std::vector<char>()) {};
	VectorMap(int width, int length) : WorldMap(width, length), vectorWorldMap(std::vector<char>(width*length)) {};
	~VectorMap();
	char get(Coordinate coord) override;
	void insert(Coordinate coord, char tile) override;
	void GenerateOneIslandMap() override;
	void GenerateFourIslandMap() override;
};