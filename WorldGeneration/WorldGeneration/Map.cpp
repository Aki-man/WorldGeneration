#include "map.h"
#include <random>
#include <tuple>

WorldMap::~WorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

const int START_OF_ISLAND = 1;
const int SEA_BORDER_SIZE = 2;
const int RANDOM_WIDTH_CHANGE = 2;
const int RANDOM_ISLAND_OFFSET = 3;
const double PERCENTAGE_FOR_NARROWING = 0.25;

std::tuple<int, int> WorldMap::generateIslandLengthAndStart(int islandLength, int islandStart, int startWidth, int endWidth, int currentLength) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> widthChangeGenerator(0, RANDOM_WIDTH_CHANGE);
	std::uniform_int_distribution<int> offsetChangeGenerator(0, RANDOM_ISLAND_OFFSET);
	int widthChange = widthChangeGenerator(rd);
	int offsetChange = offsetChangeGenerator(rd);
	if (std::rand() % 2 == 0) {
		islandLength += widthChange;
		if (islandLength > endWidth - startWidth) {
			islandLength = endWidth - startWidth - SEA_BORDER_SIZE;
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

		if (islandStart + islandLength > startWidth + (endWidth - startWidth) - (SEA_BORDER_SIZE / 2)) {
			islandStart = startWidth + (endWidth - startWidth) - islandLength - (SEA_BORDER_SIZE / 2);
		}
	}
	else {
		islandStart -= offsetChange;
		if (islandStart < startWidth + (SEA_BORDER_SIZE / 2)) {
			islandStart = startWidth + (SEA_BORDER_SIZE / 2);
		}
	}
	std::tuple<int, int> returnValue = std::make_tuple(islandLength, islandStart);
	return returnValue;
}

void WorldMap::generateIsland(int startWidth, int endWidth, int startLength, int endLength)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>  widthGenerator(0, RANDOM_WIDTH_CHANGE);
	std::uniform_int_distribution<int>  offsetGenerator(0, RANDOM_ISLAND_OFFSET);
	std::uniform_int_distribution<int>  startGenerator(0, endWidth - startWidth);
	std::uniform_int_distribution<int>  lengthGenerator(0, int((endWidth - startWidth) / 4));
	std::uniform_int_distribution<int> coinFlip(0, 1);
	int islandLength = lengthGenerator(gen);
	//int islandStart = std::rand()%(endWidth-startWidth);
	int islandStart = startGenerator(gen);
	bool generateIsland = false;
	for (int i = startLength; i < endLength; ++i) {
		if (i < START_OF_ISLAND || i > endLength - START_OF_ISLAND) {
			this->generateSeaLine(startWidth, endWidth, i);
		}
		else if (i == START_OF_ISLAND || i == endLength - START_OF_ISLAND) {
			std::tuple lengthAndStart = this->generateIslandLengthAndStart(islandLength, islandStart, startWidth, endWidth, i);
			islandLength = std::get<0>(lengthAndStart);
			islandStart = std::get<1>(lengthAndStart);
			this->generateCoastLine(startWidth, endWidth, islandStart, islandStart + islandLength, i);
		}
		else {
			std::tuple lengthAndStart = this->generateIslandLengthAndStart(islandLength, islandStart, startWidth, endWidth, i);
			islandLength = std::get<0>(lengthAndStart);
			islandStart = std::get<1>(lengthAndStart);
			this->generateIslandLine(startWidth, endWidth, islandStart, islandStart + islandLength, i);
		}
	}
}

void WorldMap::generateSeaLine(int startWidth, int endWidth, int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		this->worldMap[coord] = '~';
	}
}

void WorldMap::generateIslandLine(int startWidth, int endWidth, int island_begin, int island_end, int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		if (i == island_begin || i == island_end) {
			Coordinate coord(i, length);

			this->worldMap[coord] = 'C';
		}
		else if (i > island_begin && i < island_end) {
			Coordinate coord(i, length);

			this->worldMap[coord] = 'O';
		}
		else {
			Coordinate coord(i, length);
			this->worldMap[coord] = '~';
		}
	}
}

void WorldMap::generateCoastLine(int startWidth, int endWidth, int island_begin, int island_end, int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		if (i > island_begin && i < island_end) {
			Coordinate coord(i, length);

			this->worldMap[coord] = 'C';
		}
		else {
			Coordinate coord(i, length);
			this->worldMap[coord] = '~';
		}
	}
}

std::ostream& operator<<(std::ostream& out, WorldMap& map)
{
	for (int i = 0; i < map.length; ++i) {
		for (int j = 0; j < map.width; ++j) {
			Coordinate coord(j, i);
			char temp = map.worldMap[coord];
			if (temp == 'O') {
				out << "\033[32m";
			}
			else if (temp == '~') {
				out << "\033[34m";
			}
			else if (temp == 'C') {
				out << "\033[33m";
			}
			out << temp;
		}
		out << std::endl;
	}
	return out;
}
