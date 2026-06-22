#include "ParallelMap.h"
#include "map.h"
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>
#include "ParallelMapHelper.h"
#include "IslandGenerator.h"
#include "ParallelIslandGenerator.h"
#include <fstream>
using namespace tbb;

ParallelWorldMap::~ParallelWorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

void ParallelWorldMap::GenerateOneIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	IslandGenerator generator(&worldMap, 0, this->width, 0, this->length, config);

	generator.generateIsland();

	generator.secondPass();

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

	/*g.run([&] {generatorOne.generateIsland(); generatorTwo.generateIsland(); });

	g.run([&] {generatorThree.generateIsland(); generatorFour.generateIsland(); });
	g.wait();*/
	
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

void ParallelWorldMap::save(std::string saveName)
{
	int fileNumber = 1;
	
	task_group g;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = saveName + "Island" + std::to_string(fileNumber) + ".txt";
			g.run([=] {this->saveMapChunk(fileName, i, l); });
			
			fileNumber++;
		}
	}
	g.wait();
}

void ParallelWorldMap::saveMapChunk(std::string fileName, int i, int l) {
	
	std::ofstream file(fileName, std::ios::out | std::ios::binary);
	for (int j = i * (this->length / 2); j < (i + 1) * (this->length / 2); ++j) {
		for (int k = l * (this->width / 2); k < (l + 1) * (this->length / 2); ++k) {
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
			bool found = this->parallelWorldMap.find(a, Coordinate(j, k));
			std::string savedData;
			if (!found)
				savedData = "=";
			else
				savedData = std::to_string(j) + " " + std::to_string(k) + " " + a->second + " ";
			file << savedData;
		}
	}
	file.close();
}

bool ParallelWorldMap::load(std::string saveName)
{
	int fileNumber = 1;
	task_group g;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = saveName + "Island" + std::to_string(fileNumber) + ".txt";
			
			g.run([=]{this->loadMapChunk(fileName, i, l); });
			fileNumber++;
		}
	}
	g.wait();
	if (parallelWorldMap.size() != this->length * this->width)
		return false;
	return true;
}

void ParallelWorldMap::loadMapChunk(std::string fileName, int i, int l) {
	std::ifstream file(fileName, std::ios::out | std::ios::binary);
	while (!file.eof()) {
		int x;
		int y;
		char tile;
		file >> x;
		file >> y;
		file >> tile;
		tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
		this->parallelWorldMap.insert(a, Coordinate(x, y));
		a->second = tile;
		a.release();
	}
	file.close();
}


std::ostream& operator<<(std::ostream& out, ParallelWorldMap& map)
{
	for (int i = 0; i < map.length; ++i) {
		for (int j = 0; j < map.width; ++j) {
			Coordinate coord(j, i);

			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
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
