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
	return this->vectorWorldMap[coord.y * this->width + coord.x];
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
	VectorIslandGenerator generatorTwo(nullptr, &this->vectorWorldMap, this->width / 2, this->width, 0, this->length / 2, this->width,config);
	VectorIslandGenerator generatorThree(nullptr, &this->vectorWorldMap,0, this->width / 2, this->length / 2, this->length, this->width, config);
	VectorIslandGenerator generatorFour(nullptr, &this->vectorWorldMap,this->width / 2, this->width, this->length / 2, this->length, this->width,config);

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

void VectorMap::GenerateNIslandMap(int n, int startX, int width, int startY, int length)
{
	std::uniform_int_distribution<int> XpartitionGenerator(startX + width/6, (startX+width) - (width/6));
	std::uniform_int_distribution<int> YpartitionGenerator(startY + length / 6, (startY + length) - (length / 6));

	std::random_device rd = std::random_device();
	int y1 = YpartitionGenerator(rd);
	int y2 = YpartitionGenerator(rd);
	int x1 = XpartitionGenerator(rd);

	IslandGeneratorConfiguration config1 = IslandGeneratorConfiguration::generateConfiguration(y1 - startY, x1 - startX);
	IslandGeneratorConfiguration config2 = IslandGeneratorConfiguration::generateConfiguration(y2 - startY, (startX + width) - x1);
	IslandGeneratorConfiguration config3 = IslandGeneratorConfiguration::generateConfiguration((startY + length) - y1 , x1 - startX);
	IslandGeneratorConfiguration config4 = IslandGeneratorConfiguration::generateConfiguration((startY + length) - y2, (startX + width) - x1);



	VectorIslandGenerator generatorOne(nullptr, &this->vectorWorldMap, startX, x1, startY, y1, this->width, config1);
	VectorIslandGenerator generatorTwo(nullptr, &this->vectorWorldMap, x1, startX + width, startY, y2, this->width, config2);
	VectorIslandGenerator generatorThree(nullptr, &this->vectorWorldMap, startX, x1, y1, startY + length, this->width, config3);
	//VectorIslandGenerator generatorFour(nullptr, &this->vectorWorldMap, x1, startX + width, y2, startY + length, this->width, config4);
	if (n > 0)
		this->GenerateNIslandMap(0, x1, (startX + width) - x1, y2, (startY + length) - y2);

	tbb::task_group g;
	g.run([&] {generatorOne.generateIsland(); });
	g.run([&] {generatorTwo.generateIsland(); });
	g.run([&] {generatorThree.generateIsland(); });
	//g.run([&] {generatorFour.generateIsland(); });
	g.wait();

	tbb::task_group g2;

	g2.run([&] {generatorOne.secondPass(); });
	g2.run([&] {generatorTwo.secondPass(); });
	g2.run([&] {generatorThree.secondPass(); });
	//g2.run([&] {generatorFour.secondPass(); });

	g2.wait();
	this->isChanged = true;
}
