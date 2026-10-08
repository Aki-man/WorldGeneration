#include "ParallelMap.h"
#include "map.h"
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>
#include "ParallelMapHelper.h"
#include "IslandGenerator.h"
#include "ParallelIslandGenerator.h"
#include <fstream>
#include <filesystem>
using namespace tbb;

ParallelWorldMap::~ParallelWorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

char ParallelWorldMap::get(Coordinate coord)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
	bool found = this->parallelWorldMap.find(a, coord);
	char temp = '=';
	if (found)
		temp = a->second;
	return temp;
}

void ParallelWorldMap::insert(Coordinate coord, char tile)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
	this->parallelWorldMap.insert(a, coord);
	a->second = tile;
}

bool ParallelWorldMap::contains(Coordinate coord)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
	return this->parallelWorldMap.find(a, coord);
}

void ParallelWorldMap::GenerateOneIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	ParallelIslandGenerator generator(nullptr, &this->parallelWorldMap,0, this->width, 0, this->length, config);

	generator.generateIsland();

	generator.secondPass();
	this->isChanged = true;
}

void ParallelWorldMap::GenerateOneIslandMapParallel()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	ParallelIslandGenerator generator(nullptr, &this->parallelWorldMap, 0, this->width, 0, this->length, config);

	generator.generateIsland();

	generator.parallelSecondPass();
	this->isChanged = true;
}

void ParallelWorldMap::GenerateFourIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	ParallelIslandGenerator generatorOne(nullptr, &this->parallelWorldMap, 0, this->width / 2, 0, this->length / 2, config);
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
	this->isChanged = true;
}

void ParallelWorldMap::loadMap(int fileNumber, std::string saveName)
{
	fileNumber++;
	task_group g;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";
			if (std::filesystem::exists(fileName))
				g.run([=] {this->loadMapChunk(fileName, i, l); });
			fileNumber++;
		}
	}
	g.wait();
}


std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map)
{
	map.print(out);
	return out;
}
