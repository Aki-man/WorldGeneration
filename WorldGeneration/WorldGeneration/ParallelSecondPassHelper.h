#pragma once
#include <tbb/concurrent_hash_map.h>
#include <tbb/blocked_range.h>
#include "Coordinate.h"
#include "MyHashCompare.h"
#include <random>

#include "ParallelIslandGenerator.h"

class ParallelSecondPassHelper {
	ParallelIslandGenerator* output;
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>* parallelWorldMap;
	int* lakeNumber;
	bool* riverGenerated;
	int startWidth;
	int endWidth;
public:
	ParallelSecondPassHelper() : output(nullptr), parallelWorldMap(nullptr), lakeNumber(0), riverGenerated(nullptr),
	startWidth(0), endWidth(0){};
	ParallelSecondPassHelper(ParallelIslandGenerator* output, tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>* parallelMap,
		int* lakeNumber, bool* river, int start, int end) :
		output(output), parallelWorldMap(parallelMap), lakeNumber(lakeNumber), riverGenerated(river), startWidth(start), endWidth(end) { };
	~ParallelSecondPassHelper() { output = nullptr; lakeNumber = 0; };
	void operator()(tbb::blocked_range<size_t> range) const {
		std::random_device rd;
		for (size_t it = range.begin(); it != range.end(); ++it) {
			for (int j = startWidth; j < endWidth; ++j) {
				Coordinate coord(it, j);
				tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
				bool found = (*this->parallelWorldMap).find(a, coord);

				char tile = '=';
				if (found)
					tile = a->second;
				a.release();
				if ((*this->output).isAdjacentTo(coord, '~') && tile != '~' && tile != 'R' && tile != 'M') {
					tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor writer;
					(*this->parallelWorldMap).insert(writer, coord);
					writer->second = 'C';
					writer.release();
				}
				else if ((tile == 'O' || tile == 'T') && (*lakeNumber) > 0) {
					std::uniform_int_distribution<int> randomLakesize(1, 8);
					if (std::rand() % 20 == 0) {
						(*this->output).generateTileClump(coord, randomLakesize(rd), 'L');
						--(*lakeNumber);
					}
				}
				if (tile == 'T') {
					if (std::rand() % 3 == 0) {
						std::uniform_int_distribution<int> randomForestSize(0, 4);
						(*this->output).generateTileClump(coord, randomForestSize(rd), 'T');
					}
				}
				if ((*this->output).isAdjacentTo(coord, 'M') && !(*riverGenerated)) {
					bool goingLeft = true;
					if (std::rand() % 2 == 0)
						goingLeft = false;
					bool goingUp = true;
					if (std::rand() % 2 == 0)
						goingUp = false;
					if (std::rand() % 10 == 0) {
						(*this->output).generateRiver(coord, goingLeft, goingUp);
						(*riverGenerated) = true;
					}
				}
			}
		}
	}
};