#include "coordinate.h"
#include "MyHashCompare.h"
#include "map.h"
#include <map>
#include <cmath>
#include <string>
#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_hash_map.h>



class ParallelWorldMap : public WorldMap {
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare> worldMap;
	int width;
	int length;
public:
	ParallelWorldMap() : width(0), length(0), worldMap(tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>()) {};
	ParallelWorldMap(int width, int length) : width(width), length(length), worldMap(tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>()) {};
	~ParallelWorldMap();
	void generateIsland(int startWidth, int endWidth, int startLength, int endLength) override;
	void generateSeaLine(int startWidth, int endWidth, int length);
	void generateIslandLine(int startWidth, int endWidth, int islandLength, int islandOffset, int length) override;
	friend std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map);
};