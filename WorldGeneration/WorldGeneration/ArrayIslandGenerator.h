#pragma once
#include "IslandGenerator.h"
class ArrayIslandGenerator : public IslandGenerator {
	char* arrayWorldMap;
	int totalWidth;
	int totalSize;
public:
	ArrayIslandGenerator(std::unordered_map<Coordinate, char>* worldMap, char* arrayWorldMap, int startWidth, int endWidth, int startLength, int endLength, int totalWidth, int totalSize,IslandGeneratorConfiguration config) :
		IslandGenerator(worldMap, startWidth, endWidth, startLength, endLength, config), arrayWorldMap(arrayWorldMap), totalWidth(totalWidth), totalSize(totalSize) {
	};
	~ArrayIslandGenerator();
	virtual void insert(Coordinate coord, char tile) override;
	virtual void replace(Coordinate coord, char tile) override;
	virtual char get(Coordinate coord) override;
	virtual bool contains(Coordinate coord) override;
};