#include "ArrayMap.h"
#include "IslandGeneratorConfiguration.h"
#include "ArrayIslandGenerator.h"
#include <tbb/parallel_for.h>

ArrayMap::~ArrayMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();
	delete[] this->arrayWorldMap;
}

char ArrayMap::get(Coordinate coord)
{
	return this->arrayWorldMap[coord.y * this->width + coord.x];
}

void ArrayMap::insert(Coordinate coord, char tile)
{
	this->arrayWorldMap[coord.y * this->width + coord.x] = tile;
}

void ArrayMap::GenerateOneIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	ArrayIslandGenerator generator(nullptr, this->arrayWorldMap, 0, this->width, 0, this->length, this->width, this->width*this->length,config);

	generator.generateIsland();

	generator.secondPass();
}

void ArrayMap::GenerateFourIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	ArrayIslandGenerator generatorOne(nullptr, this->arrayWorldMap, 0, this->width / 2, 0, this->length / 2, this->width, this->width * this->length, config);
	//generatorOne.generateIsland();
	ArrayIslandGenerator generatorTwo(nullptr, this->arrayWorldMap, this->width / 2, this->width, 0, this->length / 2, this->width, this->width * this->length, config);
	//generatorTwo.generateIsland();
	ArrayIslandGenerator generatorThree(nullptr, this->arrayWorldMap, 0, this->width / 2, this->length / 2, this->length, this->width, this->width * this->length, config);
	//.generateIsland();
	ArrayIslandGenerator generatorFour(nullptr, this->arrayWorldMap, this->width / 2, this->width, this->length / 2, this->length, this->width, this->width * this->length, config);
	//generatorFour.generateIsland();

	/*generatorOne.secondPass();
	generatorTwo.secondPass();
	generatorThree.secondPass();
	generatorFour.secondPass();*/

	tbb::task_group g;
	g.run([&] {generatorOne.generateIsland(); });
	g.run([&] {generatorTwo.generateIsland(); });
	g.run([&] {generatorThree.generateIsland(); });
	g.run([&] {generatorFour.generateIsland(); });
	g.wait();

	tbb::task_group g2;

	g2.run([&] {generatorOne.secondPass(); });
	g2.run([&] {generatorTwo.secondPass(); });
	g2.run([&] {generatorThree.secondPass(); });
	g2.run([&] {generatorFour.secondPass(); });

	g2.wait();
}
