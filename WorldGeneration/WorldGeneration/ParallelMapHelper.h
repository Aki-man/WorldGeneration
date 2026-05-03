#pragma once
#include <tbb/concurrent_hash_map.h>
#include <tbb/blocked_range.h>
#include "coordinate.h"
#include "MyHashCompare.h"

class ParallelWorldMapHelper {
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>* output;
	int island_begin;
	int island_end;
	int length;
public:
	ParallelWorldMapHelper() : output(nullptr), island_begin(0), island_end(0), length(0) {};
	ParallelWorldMapHelper(tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>* map, int begin, int end, int length) : output(map), island_begin(begin)
		, island_end(end), length(length) {
	};
	~ParallelWorldMapHelper() { output = nullptr; island_begin = 0; island_end = 0; length = 0; };
	void operator() (tbb::blocked_range<size_t> range) const {
		for (size_t it = range.begin(); it != range.end(); ++it) {
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			if (it > island_begin && it < island_end) {
				Coordinate coord(it, length);

				(*output).insert(a, coord);
				a->second = 'O';
			}
			else {
				Coordinate coord(it, length);
				(*output).insert(a, coord);
				a->second = '~';
			}
		}
	}
};