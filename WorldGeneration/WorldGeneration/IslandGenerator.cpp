#include "IslandGenerator.h"
#include "IslandGeneratorConfiguration.h"
#include <random>

IslandGenerator::~IslandGenerator()
{
	this->worldMap = nullptr;
	this->startWidth = 0;
	this->endWidth = 0;
	this->startLength = 0;
	this->endLength = 0;
	

}

std::tuple<int, int> IslandGenerator::generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength) {

	
	std::uniform_int_distribution<int> widthChangeGenerator(0, config.randomIslandWidthChange);
	std::uniform_int_distribution<int> offsetChangeGenerator(0, config.randomIslandOffsetChange);
	int widthChange = widthChangeGenerator(rd);
	int offsetChange = offsetChangeGenerator(rd);
	if (std::rand() % 2 == 0) {
		islandLength += widthChange;
		if (islandLength > endWidth - startWidth) 
			islandLength = endWidth - startWidth - config.mapSeaBorderSize;
	}
	else {
		islandLength -= widthChange;
		if (islandLength < 0) 
			islandLength = 1;
	}
	if (std::rand() % 2 == 0) {
		islandStart += offsetChange;

		if (islandStart + islandLength > startWidth + (endWidth - startWidth) - (config.mapSeaBorderSize / 2)) 
			islandStart = startWidth + (endWidth - startWidth) - islandLength - (config.mapSeaBorderSize / 2);
	}
	else {
		islandStart -= offsetChange;
		if (islandStart < startWidth + (config.mapSeaBorderSize / 2)) 
			islandStart = startWidth + (config.mapSeaBorderSize / 2);
	}
	std::tuple<int, int> returnValue = std::make_tuple(islandLength, islandStart);
	return returnValue;
}

std::tuple<int, int> IslandGenerator::generateMountainLengthAndStart(std::tuple<int, int> islandLengthAndStart, int mountainLength, int mountainStart)
{
	int islandLength = std::get<0>(islandLengthAndStart);
	int islandStart = std::get<1>(islandLengthAndStart);
	int islandEnd = islandStart + islandLength;
	std::uniform_int_distribution<int> widthChangeGenerator(0, config.randomMountainWidthChange);
	std::uniform_int_distribution<int> offsetChangeGenerator(0, config.randomMountainOffsetChange);
	int widthChange = widthChangeGenerator(rd);
	int offsetChange = offsetChangeGenerator(rd);
	if (std::rand() % 2 == 0) {
		mountainLength += widthChange;
		if (mountainLength > islandLength)
			mountainLength = islandLength;
	}
	else {
		mountainLength -= widthChange;
		if (mountainLength < 0)
			mountainLength = 0;
	}
	if (std::rand() % 2 == 0) {
		mountainStart += offsetChange;

		if (mountainStart + mountainLength > islandStart +  islandLength)
			mountainStart = islandStart + islandLength - mountainLength;
	}
	else {
		mountainStart -= offsetChange;
		if (mountainStart < islandStart)
			mountainStart = islandStart;
	}
	std::tuple<int, int> returnValue = std::make_tuple(mountainLength, mountainStart);
	return returnValue;
}



void IslandGenerator::generateIsland()
{
	std::uniform_int_distribution<int>  startGenerator(0, endWidth - startWidth);
	std::uniform_int_distribution<int>  lengthGenerator(0, int((endWidth - startWidth) / 4));
	std::uniform_int_distribution<int> mountainGenerationStartGenerator(startLength, endLength);
	
	int mountainStartOfGeneration = mountainGenerationStartGenerator(rd);
	std::uniform_int_distribution<int> mountainGenerationEndGenerator(mountainStartOfGeneration, endLength);
	int mountainEndOfGeneration = mountainGenerationEndGenerator(rd);
	int islandLength = lengthGenerator(rd);
	
	int islandStart = startGenerator(rd);
	int mountainStart = 0;
	int mountainLength = 0;
	bool generateIsland = false;
	bool generateMountain = false;
	for (int i = startLength; i < endLength; ++i) {
		if (i < config.islandGenerationStart || i > endLength - config.islandGenerationStart) {
			this->generateSeaLine(i);
		}
		else{
			std::tuple lengthAndStart = this->generateIslandLengthAndStart(islandLength, islandStart, i);
			islandLength = std::get<0>(lengthAndStart);
			islandStart = std::get<1>(lengthAndStart);
			if(!generateMountain)
				this->generateIslandLine(islandStart, islandStart + islandLength, i);
			else {
				std::tuple mountainLengthAndStart = this->generateMountainLengthAndStart(lengthAndStart, mountainLength, mountainStart);
				mountainLength = std::get<0>(mountainLengthAndStart);
				mountainStart = std::get<1>(mountainLengthAndStart);
				this->generateIslandLineWithMountain(islandStart, islandStart + islandLength, i, mountainStart, mountainStart + mountainLength);
				if (i > mountainEndOfGeneration)
					generateMountain = false;
			}
		}
		if (i > mountainStartOfGeneration) {
			if (std::rand() % 4 == 0) {
				generateMountain = true;
				mountainStart = islandStart + 2;
				mountainLength = 3;
			}
		}
	}
}

void IslandGenerator::secondPass() {
	for (int i = startLength; i < endLength; ++i) {
		for (int j = startWidth; j < endWidth; ++j) {
			Coordinate coord(i, j);
			if (this->isAdjacentTo(coord, '~') && (*this->worldMap)[coord] != '~') {
				(*this->worldMap)[coord] = 'C';
			}
		}
	}
}

bool IslandGenerator::isAdjacentTo(Coordinate coord, char tile) {

	Coordinate leftAdjacentTile = Coordinate(coord.x - 1, coord.y);
	Coordinate rightAdjacentTile = Coordinate(coord.x + 1, coord.y);
	Coordinate upAdjacentTile = Coordinate(coord.x, coord.y + 1);
	Coordinate downAdjacentTile = Coordinate(coord.x, coord.y - 1);
	std::vector<Coordinate> coordinatesToCheck = { leftAdjacentTile, rightAdjacentTile, upAdjacentTile, downAdjacentTile };
	for (Coordinate coord : coordinatesToCheck)
	{
		if ((*this->worldMap).contains(coord)) {
			char foundTile = (*worldMap)[coord];
			if (foundTile == tile) {
				return true;
			}
		}
	}
	return false;
}

void IslandGenerator::generateSeaLine(int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		(*this->worldMap)[coord] = '~';
	}
}

void IslandGenerator::generateIslandLine(int island_begin, int island_end, int length)
{
	std::uniform_int_distribution<int> forestChanceGenerator(0, config.forestSpawnChance);
	for (int i = startWidth; i < endWidth; ++i) {
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);
			int forest = forestChanceGenerator(rd);
			if (forest == config.forestSpawnChance)
				(*this->worldMap)[coord] = 'T';
			else
				(*this->worldMap)[coord] = 'O';
		}
		else {
			Coordinate coord(i, length);
			(*this->worldMap)[coord] = '~';
		}
	}
}

void IslandGenerator::generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end)
{
	std::uniform_int_distribution<int> forestChanceGenerator(0, config.forestSpawnChance);
	for (int i = startWidth; i < endWidth; ++i) {
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);
			if (i > mountain_begin && i <= mountain_end) {
				(*this->worldMap)[coord] = 'M';
				continue;
			}
			int forest = forestChanceGenerator(rd);
			if (forest == config.forestSpawnChance)
				(*this->worldMap)[coord] = 'T';
			else
				(*this->worldMap)[coord] = 'O';
		}
		else {
			Coordinate coord(i, length);
			(*this->worldMap)[coord] = '~';
		}
	}
}



