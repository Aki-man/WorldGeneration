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
	std::map<Coordinate, char> worldMap;
	int width;
	int length;
public:
	ParallelWorldMap() : width(0), length(0), worldMap(std::map<Coordinate, char>()) {};
	ParallelWorldMap(int width, int length) : width(width), length(length), worldMap(std::map<Coordinate, char>()) {};
	~ParallelWorldMap();
	void GenerateFourIslandMap() override;
	void print(std::ostream& out) override;
	friend std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map);
};