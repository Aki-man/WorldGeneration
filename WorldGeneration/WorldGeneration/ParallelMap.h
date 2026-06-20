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
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare> parallelWorldMap;
public:
	ParallelWorldMap() : WorldMap(), parallelWorldMap(tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>()) {};
	ParallelWorldMap(int width, int length) : WorldMap(width, length) {};
	~ParallelWorldMap();
	void GenerateFourIslandMap() override;
	//using WorldMap::print;
	void print(std::ostream& out) override;
	friend std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map);
};