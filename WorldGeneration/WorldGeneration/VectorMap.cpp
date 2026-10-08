#include "VectorMap.h"
#include "IslandGeneratorConfiguration.h"
#include "VectorIslandGenerator.h"
#include <tbb/parallel_for.h>

VectorMap::~VectorMap()
{
	this->width = 0;
	this->length = 0;
	this->worldMap.clear();
	this->vectorWorldMap.clear();
}

char VectorMap::get(Coordinate coord)
{
	//int x = coord.x;
	//int y = coord.y;
	return this->vectorWorldMap[coord.y*this->width + coord.x];
}

void VectorMap::insert(Coordinate coord, char tile)
{
	this->vectorWorldMap[coord.y * this->width + coord.x] = tile;
}

bool VectorMap::contains(Coordinate coord)
{
	return this->get(coord) != '\0';
}

void VectorMap::GenerateOneIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	VectorIslandGenerator generator(nullptr, &this->vectorWorldMap, 0, this->width, 0, this->length, this->width, config);

	generator.generateIsland();

	generator.secondPass();
	this->isChanged = true;
}

void VectorMap::GenerateFourIslandMap()
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(this->length / 2, this->width / 2);
	VectorIslandGenerator generatorOne(nullptr, &this->vectorWorldMap,0, this->width / 2, 0, this->length / 2, this->width,config);
	//generatorOne.generateIsland();
	VectorIslandGenerator generatorTwo(nullptr, &this->vectorWorldMap, this->width / 2, this->width, 0, this->length / 2, this->width,config);
	//generatorTwo.generateIsland();
	VectorIslandGenerator generatorThree(nullptr, &this->vectorWorldMap,0, this->width / 2, this->length / 2, this->length, this->width, config);
	//generatorThree.generateIsland();
	VectorIslandGenerator generatorFour(nullptr, &this->vectorWorldMap,this->width / 2, this->width, this->length / 2, this->length, this->width,config);
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
	this->isChanged = true;
}
