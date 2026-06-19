#include "map.h"
#include "IslandGenerator.h"
#include "IslandGeneratorConfiguration.h"
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
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration();
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
			else if (temp == '~') {
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


std::ostream& operator<<(std::ostream& out, WorldMap& map)
{
	map.print(out);
	return out;

}
