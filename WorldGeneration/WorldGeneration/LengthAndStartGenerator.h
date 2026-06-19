#pragma once
#include <tuple>
#include "IslandGeneratorConfiguration.h"
#include "IslandGenerator.h"
class LengthAndStartGenerator {
	int startWidth;
	int endWidth;
	std::random_device& rd;
	IslandGeneratorConfiguration& config;
public:
	LengthAndStartGenerator(int startWidth, int endWidth, std::random_device& rd, IslandGeneratorConfiguration& config) 
		: startWidth(startWidth), endWidth(endWidth), rd(rd), config(config) {};
	std::tuple<int, int> generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength, bool getWider);
	std::tuple<int, int> generateMountainLengthAndStart(std::tuple<int, int> islandLengthAndStart, int mountainLength, int mountainStart);
};