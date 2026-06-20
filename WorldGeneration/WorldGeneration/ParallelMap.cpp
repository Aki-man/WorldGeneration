#include "ParallelMap.h"
#include "map.h"
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>
#include "ParallelMapHelper.h"
#include "IslandGenerator.h"
#include "ParallelIslandGenerator.h"
using namespace tbb;

ParallelWorldMap::~ParallelWorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

void ParallelWorldMap::GenerateFourIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	ParallelIslandGenerator generatorOne(nullptr, &this->parallelWorldMap,0, this->width / 2, 0, this->length / 2, config);
	ParallelIslandGenerator generatorTwo(nullptr, &this->parallelWorldMap, this->width / 2, this->width, 0, this->length / 2, config);
	ParallelIslandGenerator generatorThree(nullptr, &this->parallelWorldMap, 0, this->width / 2, this->length / 2, this->length, config);
	ParallelIslandGenerator generatorFour(nullptr, &this->parallelWorldMap, this->width / 2, this->width, this->length / 2, this->length, config);
	
	task_group g;
	g.run([&] {generatorOne.generateIsland(); });
	g.run([&] {generatorTwo.generateIsland(); });
	g.run([&] {generatorThree.generateIsland(); });
	g.run([&] {generatorFour.generateIsland(); });
	g.wait();

	task_group g2;
	g2.run([&] {generatorOne.secondPass(); });
	g2.run([&] {generatorTwo.secondPass(); });
	g2.run([&] {generatorThree.secondPass(); });
	g2.run([&] {generatorFour.secondPass(); });
	g2.wait();
}


void ParallelWorldMap::print(std::ostream& out)
{
	WorldMap::print(out);
}


std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map)
{
	for (int i = 0; i < map.length; ++i) {
		for (int j = 0; j < map.width; ++j) {
			Coordinate coord(j, i);

			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			bool found = map.parallelWorldMap.find(a, coord);
			char temp = '=';
			if(found)
				temp = a->second;
			if (temp == 'O') {
				out << "\033[32m";
			}
			else if (temp == 'T') {
				out << "\033[38;5;22m";
			}
			else if (temp == '~' || temp == 'L' || temp == 'R') {
				out << "\033[34m";
			}
			else if (temp == 'C') {
				out << "\033[33m";
			}
			else {
				out << "\x1b[0m";
			}
			out << temp;
		}
		out << std::endl;
	}
	for (int i = 0; i < map.length; ++i) {
		out << '=';
	}
	out << std::endl;
	return out;
}
