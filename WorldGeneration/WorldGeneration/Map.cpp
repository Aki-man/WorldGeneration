#include "map.h"
#include "IslandGenerator.h"
#include "IslandGeneratorConfiguration.h"
#include <fstream>
#include <filesystem>
#include <random>
#include <tuple>
#include <tbb/parallel_for.h>

WorldMap::~WorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

}

void WorldMap::insert(Coordinate coord, char tile)
{
	this->worldMap[coord] = tile;
}

char WorldMap::get(Coordinate coord)
{
	return worldMap[coord];
}

bool WorldMap::contains(Coordinate coord)
{
	return this->worldMap.contains(coord);
}

void WorldMap::GenerateOneIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	IslandGenerator generator(&worldMap, 0, this->width, 0, this->length, config);

	generator.generateIsland();

	generator.secondPass();
	this->isChanged = true;

}

void WorldMap::GenerateFourIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length/2, this->width/2);
	IslandGenerator generatorOne(&worldMap, 0,this->width/2, 0,this->length/2, config);
	generatorOne.generateIsland();
	IslandGenerator generatorTwo(&this->worldMap, this->width/2, this->width, 0, this->length / 2, config);
	generatorTwo.generateIsland();
	IslandGenerator generatorThree(&this->worldMap, 0, this->width / 2, this->length / 2, this->length, config);
	generatorThree.generateIsland();
	IslandGenerator generatorFour(&this->worldMap, this->width/2, this->width, this->length/2, this->length, config);
	generatorFour.generateIsland();

	generatorOne.secondPass();
	generatorTwo.secondPass();
	generatorThree.secondPass();
	generatorFour.secondPass();
	this->isChanged = true;
}

void WorldMap::print(std::ostream& out)
{
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			Coordinate coord(j, i);
			char temp = this->get(coord);
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
	for (int i = 0; i < length; ++i) {
		out << '=';
	}
	out << std::endl;
	
}

void WorldMap::save(std::string saveName)
{
	std::filesystem::path path = "data/saves/" + saveName;
	if (!std::filesystem::is_directory(path))
		std::filesystem::create_directory(path);
	int fileNumber = 0;
	std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";
	std::ofstream file(fileName, std::ios::out | std::ios::binary);
	file << std::to_string(this->width) << " " << std::to_string(this->length);
	file.close();

	fileNumber++;
	tbb::task_group g;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";

			g.run([=] {this->saveMapChunk(fileName, i, l); });

			fileNumber++;
		}
	}
	g.wait();
	this->isChanged = false;
}

void WorldMap::parallelSave(std::string saveName) {
	std::filesystem::path path = "data/saves/" + saveName;
	if (!std::filesystem::is_directory(path))
		std::filesystem::create_directory(path);
	int fileNumber = 0;
	std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";
	std::ofstream file(fileName, std::ios::out | std::ios::binary);
	file << std::to_string(this->width) << " " << std::to_string(this->length);
	file.close();

	fileNumber++;
	tbb::task_group g;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";
			 
			g.run([=] {this->saveMapChunk(fileName, i, l) ; });

			fileNumber++;
		}
	}
	g.wait();
}

void WorldMap::saveMapChunk(std::string fileName, int i, int l) {

	std::ofstream file(fileName, std::ios::out | std::ios::binary);
	char currentTile = this->get(Coordinate(i * (this->length / 2), l * (this->width / 2)));
	int currentCount = 0;
	for (int j = i * (this->length / 2); j < (i + 1) * (this->length / 2); ++j) {
		for (int k = l * (this->width / 2); k < (l + 1) * (this->length / 2); ++k) {
			char newTile = this->get(Coordinate(k, j));
			if (newTile != currentTile) {
				std::string savedData = std::to_string(currentCount) + " " + currentTile + " ";
				currentTile = newTile;
				currentCount = 0;
				file << savedData;
			}
			currentCount++;
		}
	}
	std::string savedData = std::to_string(currentCount) + " " + currentTile + " ";
	file << savedData;
	file.close();
}

bool WorldMap::load(std::string saveName) {
	this->name = saveName;
	int fileNumber = 0;
	std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";
	if (std::filesystem::exists(fileName)) {
		std::ifstream file(fileName, std::ios::out | std::ios::binary);
		file >> this->length;
		file >> this->width;
		file.close();
	}
	this->loadMap(fileNumber, saveName);
	if (worldMap.size() != this->length * this->width)
		return false;
	return true;
}

void WorldMap::loadMap(int fileNumber, std::string saveName)
{
	fileNumber++;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = "data/saves/" + saveName + "/save" + std::to_string(fileNumber) + ".txt";
			if (std::filesystem::exists(fileName))
				this->loadMapChunk(fileName, i, l);

			fileNumber++;
		}
	}
}

void WorldMap::loadMapChunk(std::string fileName, int i, int l)
{
	std::ifstream file(fileName, std::ios::out | std::ios::binary);
	char currentTile = '=';
	int currentCount = 0;
	file >> currentCount;
	file >> currentTile;
	for (int j = i * (this->length / 2); j < (i + 1) * (this->length / 2); ++j) {
		for (int k = l * (this->width / 2); k < (l + 1) * (this->length / 2); ++k) {
			if (currentCount != 0) {
				this->insert(Coordinate(k, j), currentTile);
				currentCount--;
			}
			else {
				file >> currentCount;
				file >> currentTile;
				this->insert(Coordinate(k, j), currentTile);
				currentCount--;
			}
			
		}
	}
	file.close();
	/*std::ifstream file(fileName, std::ios::out | std::ios::binary);
	while (!file.eof()) {
		int x;
		int y;
		char tile;
		file >> x;
		file >> y;
		file >> tile;
		this->insert(Coordinate(x, y), tile);
	}
	file.close();*/
}


std::ostream& operator<<(std::ostream& out, WorldMap& map)
{
	map.print(out);
	return out;

}
