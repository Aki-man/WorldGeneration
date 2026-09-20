#pragma once
#include "Map.h"
class ArrayMap : public WorldMap {
public:
	char* arrayWorldMap;
	ArrayMap() : WorldMap(), arrayWorldMap() {};
	ArrayMap(int width, int length) : WorldMap(width, length)/*, arrayWorldMap(new char[width * length])*/ {
		arrayWorldMap = new char[width * length];
	};
	~ArrayMap();
	char get(Coordinate coord) override;
	void insert(Coordinate coord, char tile) override;
	bool contains(Coordinate coord) override;
	void GenerateOneIslandMap() override;
	void GenerateFourIslandMap() override;
};