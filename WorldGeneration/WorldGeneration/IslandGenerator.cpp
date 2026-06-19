#include "IslandGenerator.h"
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

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> widthChangeGenerator(0, config.randomIslandWidthChange);
	std::uniform_int_distribution<int> offsetChangeGenerator(0, config.randomIslandOffsetChange);
	int widthChange = widthChangeGenerator(rd);
	int offsetChange = offsetChangeGenerator(rd);
	if (std::rand() % 2 == 0) {
		islandLength += widthChange;
		if (islandLength > endWidth - startWidth) {
			islandLength = endWidth - startWidth - config.mapSeaBorderSize;
		}
	}
	else {
		islandLength -= widthChange;
		if (islandLength < 0) {
			islandLength = 1;
		}
	}
	if (std::rand() % 2 == 0) {
		islandStart += offsetChange;

		if (islandStart + islandLength > startWidth + (endWidth - startWidth) - (config.mapSeaBorderSize / 2)) {
			islandStart = startWidth + (endWidth - startWidth) - islandLength - (config.mapSeaBorderSize / 2);
		}
	}
	else {
		islandStart -= offsetChange;
		if (islandStart < startWidth + (config.mapSeaBorderSize / 2)) {
			islandStart = startWidth + (config.mapSeaBorderSize / 2);
		}
	}
	std::tuple<int, int> returnValue = std::make_tuple(islandLength, islandStart);
	return returnValue;
}

void IslandGenerator::generateIsland()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>  widthGenerator(0, config.randomIslandWidthChange);
	std::uniform_int_distribution<int>  offsetGenerator(0, config.randomIslandOffsetChange);
	std::uniform_int_distribution<int>  startGenerator(0, endWidth - startWidth);
	std::uniform_int_distribution<int>  lengthGenerator(0, int((endWidth - startWidth) / 4));
	std::uniform_int_distribution<int> coinFlip(0, 1);
	int islandLength = lengthGenerator(gen);
	//int islandStart = std::rand()%(endWidth-startWidth);
	int islandStart = startGenerator(gen);
	bool generateIsland = false;
	for (int i = startLength; i < endLength; ++i) {
		if (i < config.islandGenerationStart || i > endLength - config.islandGenerationStart) {
			this->generateSeaLine(i);
		}
		else if (i == config.islandGenerationStart || i == endLength - config.islandGenerationStart) {
			std::tuple lengthAndStart = this->generateIslandLengthAndStart(islandLength, islandStart, i);
			islandLength = std::get<0>(lengthAndStart);
			islandStart = std::get<1>(lengthAndStart);
			this->generateCoastLine(islandStart, islandStart + islandLength, i);
		}
		else {
			std::tuple lengthAndStart = this->generateIslandLengthAndStart(islandLength, islandStart, i);
			islandLength = std::get<0>(lengthAndStart);
			islandStart = std::get<1>(lengthAndStart);
			this->generateIslandLine(islandStart, islandStart + islandLength, i);
		}
	}
	//this->secondPass(startWidth, endWidth, startLength, endLength);
}

void IslandGenerator::secondPass() {
	for (int i = startLength; i < endLength; ++i) {
		for (int j = startWidth; j < endWidth; ++j) {
			Coordinate coord(i, j);
			if (this->isAdjacentTo(coord, '~') && (*this->worldMap)[coord] != '~') {
				(*this->worldMap)[coord] = 'C';
			}
			//this->worldMap[coord] = 'B';
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
	for (int i = startWidth; i < endWidth; ++i) {
		/*if (i == island_begin || i == island_end) {
			Coordinate coord(i, length);

			(*this->worldMap)[coord] = 'C';
		}*/
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);

			(*this->worldMap)[coord] = 'O';
		}
		else {
			Coordinate coord(i, length);
			(*this->worldMap)[coord] = '~';
		}
	}
}

void IslandGenerator::generateCoastLine(int island_begin, int island_end, int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		if (i > island_begin && i < island_end) {
			Coordinate coord(i, length);

			(*this->worldMap)[coord] = 'C';
		}
		else {
			Coordinate coord(i, length);
			(*this->worldMap)[coord] = '~';
		}
	}
}
