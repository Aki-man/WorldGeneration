#include "ParallelMap.h"
#include "map.h"
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>
#include "ParallelMapHelper.h"
using namespace tbb;

ParallelWorldMap::~ParallelWorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

const int START_OF_ISLAND = 4;
const int SEA_BORDER_SIZE = 4;
const int RANDOM_WIDTH_CHANGE = 2;
const int RANDOM_ISLAND_OFFSET = 5;

void ParallelWorldMap::generateIsland(int startWidth, int endWidth, int startLength, int endLength)
{


	/*std::random_device rd;
	std::mt19937 gen(rd);
	std::uniform_int_distribution<int>  widthGenerator(0, RANDOM_WIDTH_CHANGE);
	std::uniform_int_distribution<int>  offsetGenerator(0, RANDOM_ISLAND_OFFSET);
	std::uniform_int_distribution<int>  startGenerator(0, endWidth-startWidth);*/
	std::uniform_int_distribution<int> coinFlip(0, 1);
	int islandLength = 10;
	int islandStart = startWidth + (std::rand() % (endWidth - startWidth));
	for (int i = startLength; i < endLength; ++i) {
		if (i < START_OF_ISLAND || i > endLength - START_OF_ISLAND) {
			this->generateSeaLine(startWidth, endWidth, i);
		}
		else {
			int widthChange = std::rand() % RANDOM_WIDTH_CHANGE;
			int offsetChange = std::rand() % RANDOM_ISLAND_OFFSET;
			if (std::rand() % 2 == 0) {
				islandLength += widthChange;
				if (islandLength > endWidth - startWidth) {
					islandLength = endWidth - startWidth - SEA_BORDER_SIZE;
				}
			}
			else {
				islandLength -= widthChange;
				if (islandLength < 0) {
					islandLength = 0;
				}
			}
			if (std::rand() % 2 == 0) {
				islandStart += offsetChange;
				if (islandStart + islandLength > startWidth + (endWidth - startWidth)) {
					islandStart = startWidth + (endWidth - startWidth) - islandLength;
				}
			}
			else {
				islandStart -= offsetChange;
				if (islandStart < startWidth + (SEA_BORDER_SIZE / 2)) {
					islandStart = startWidth + (SEA_BORDER_SIZE / 2);
				}
			}
			ParallelWorldMapHelper cwmh(&worldMap, islandStart, islandStart + islandLength, i);
			parallel_for(blocked_range<size_t>(startWidth, endWidth, (endWidth - startWidth) / 4), cwmh);
			//this->generateIslandLine(startWidth, endWidth, islandLength, islandStart, i);
		}
	}
}

void ParallelWorldMap::generateSeaLine(int startWidth, int endWidth, int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
		worldMap.insert(a, coord);
		a->second = '~';
	}
}

void ParallelWorldMap::generateIslandLine(int startWidth, int endWidth, int islandLength, int islandOffset, int length)
{
	int islandTilesBuffer = islandLength;
	for (int i = startWidth; i < endWidth; ++i) {
		if (i > islandOffset && islandTilesBuffer > 0) {
			Coordinate coord(i, length);
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			worldMap.insert(a, coord);
			a->second = 'O';
			//this->worldMap[coord] = 'O';
			--islandTilesBuffer;
		}
		else {
			Coordinate coord(i, length);
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			worldMap.insert(a, coord);
			a->second = '~';
			//this->worldMap[coord] = '~';
		}
	}
}

std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map)
{

	for (int i = 0; i < map.length; ++i) {
		for (int j = 0; j < map.width; ++j) {
			Coordinate coord(j, i);
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			map.worldMap.find(a, coord);
			char temp = a->second;
			if (temp == 'O') {
				out << "\033[32m";
			}
			else if (temp == '~') {
				out << "\033[34m";
			}
			out << temp;
		}
		out << std::endl;
	}
	return out;
}
