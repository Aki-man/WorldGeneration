#include "ParallelMap.h"
#include "map.h"
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>
#include "ParallelMapHelper.h"
#include "IslandGenerator.h"
using namespace tbb;

ParallelWorldMap::~ParallelWorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

void ParallelWorldMap::GenerateFourIslandMap()
{
	IslandGenerator generatorOne(&worldMap, 0, this->width / 2, 0, this->length / 2);
	IslandGenerator generatorTwo(&this->worldMap, this->width / 2, this->width, 0, this->length / 2);
	IslandGenerator generatorThree(&this->worldMap, 0, this->width / 2, this->length / 2, this->length);
	IslandGenerator generatorFour(&this->worldMap, this->width / 2, this->width, this->length / 2, this->length);
	
	task_group g;
	g.run([&] {generatorOne.generateIsland(); });
	g.run([&] {generatorTwo.generateIsland(); });
	g.run([&] {generatorThree.generateIsland(); });
	g.run([&] {generatorFour.generateIsland(); });
	g.wait();

	g.run([&] {generatorOne.secondPass(); });
	g.run([&] {generatorTwo.secondPass(); });
	g.run([&] {generatorThree.secondPass(); });
	g.run([&] {generatorFour.secondPass(); });
	g.wait();

}

void ParallelWorldMap::print(std::ostream& out)
{
	WorldMap::print(out);
}


std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map)
{
	map.print(out);
	return out;
}
