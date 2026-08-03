#pragma once
#include "IslandGenerator.h"
class VectorIslandGenerator : public IslandGenerator {
	std::vector<char>* vectorWorldMap;
	int totalWidth;
public:
	VectorIslandGenerator(std::unordered_map<Coordinate, char>* worldMap, std::vector<char>* vectorWorldMap, int startWidth, int endWidth, int startLength, int endLength, int totalWidth,IslandGeneratorConfiguration config) :
		IslandGenerator(worldMap, startWidth, endWidth, startLength, endLength, config), vectorWorldMap(vectorWorldMap), totalWidth(totalWidth) {
	};
	~VectorIslandGenerator();
	virtual void insert(Coordinate coord, char tile) override;
	virtual void replace(Coordinate coord, char tile) override;
	virtual char get(Coordinate coord) override;
	virtual bool contains(Coordinate coord) override;
};