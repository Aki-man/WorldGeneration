#include "map.h"
#include "IslandGenerator.h"
#include "IslandGeneratorConfiguration.h"
#include <fstream>
#include <filesystem>
#include <random>
#include <tuple>

WorldMap::~WorldMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();

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
	//map.generateIsland(0, 50, 0, 50);
	//map.generateIsland(50, 100, 0, 50);
	//map.generateIsland(0, 50, 50, 100);
	//map.generateIsland(50, 100, 50, 100);
}

void WorldMap::print(std::ostream& out)
{
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			Coordinate coord(j, i);
			char temp = worldMap[coord];
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
	int fileNumber = 1;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = saveName + "Island" + std::to_string(fileNumber) + ".txt";
			std::ofstream file(fileName, std::ios::out | std::ios::binary);
			for (int j = i * (this->length / 2); j < (i + 1) * (this->length / 2); ++j) {
				for (int k = l * (this->width / 2); k < (l + 1) * (this->length / 2); ++k) {
					std::string savedData = std::to_string(j) + " " + std::to_string(k) + " " + this->worldMap[Coordinate(j, k)] + " ";
					file << savedData;
				}	
			}
			file.close();
			fileNumber++;
		}
	}
	
}

void WorldMap::load(std::string saveName) {
	int fileNumber = 1;
	for (int i = 0; i < 2; ++i) {
		for (int l = 0; l < 2; ++l) {
			std::string fileName = saveName + "Island" + std::to_string(fileNumber) + ".txt";
			std::ifstream file(fileName, std::ios::out | std::ios::binary);
			while (!file.eof()) {
				int x;
				int y;
				char tile;
				file >> x;
				file >> y;
				file >> tile;
				worldMap[Coordinate(x, y)] = tile;
			}
			fileNumber++;
		}
	}
	if (worldMap.size() != this->length * this->width)
	{
		std::cout << "World map is of the wrong size!" << std::endl;
		std::cout << worldMap.size() << std::endl;
	}
}


std::ostream& operator<<(std::ostream& out, WorldMap& map)
{
	map.print(out);
	return out;

}
