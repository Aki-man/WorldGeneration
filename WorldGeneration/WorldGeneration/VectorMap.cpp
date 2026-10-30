#include "VectorMap.h"
#include "IslandGeneratorConfiguration.h"
#include "VectorIslandGenerator.h"
#include <tbb/parallel_for.h>
#include "Square.h"

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

void VectorMap::GenerateNIslandMap(int n, int startX, int endX, int startY, int endY)
{
	int width = endX - startX;
	int length = endY - startY;
	std::uniform_int_distribution<int> XpartitionGenerator(startX + width/6, (endX) - (width/6));
	std::uniform_int_distribution<int> YpartitionGenerator(startY + length / 6, (endY) - (length / 6));

	int recursionNumber = n / 4;
	std::random_device rd = std::random_device();
	int x1 = XpartitionGenerator(rd);
	int y1 = YpartitionGenerator(rd);
	int y2 = YpartitionGenerator(rd);

	std::vector<Square> squares = {
		Square(startX, x1, startY, y1),
		Square(x1, endX, startY, y2),
		Square(startX, x1, y1, endY),
		Square(x1, endX, y2, endY)
	};
	std::vector<VectorIslandGenerator> generators = std::vector<VectorIslandGenerator>();
	if (n - 4 > 0) {
		std::uniform_int_distribution<int> randomSquareSelector(0, 3);
		int recursionSquare = randomSquareSelector(rd);
		Square selectedSquare = squares[recursionSquare];
		std::erase(squares, selectedSquare);

		this->GenerateNIslandMap(n - 3, selectedSquare.begin.x, selectedSquare.end.x, selectedSquare.begin.y, selectedSquare.end.y);
		for (int i = 0; i < 3; i++) {
			std::uniform_int_distribution<int> randomSquareSelector(0, squares.size() - 1);
			int recursionSquare = randomSquareSelector(rd);
			std::cout << "selected: " << recursionSquare << std::endl;
			Square selectedSquare = squares[recursionSquare];
			
			IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(selectedSquare.end.y - selectedSquare.begin.y, selectedSquare.end.x - selectedSquare.begin.x);
			VectorIslandGenerator generator(nullptr, &this->vectorWorldMap, selectedSquare.begin.x, selectedSquare.end.x, selectedSquare.begin.y, selectedSquare.end.y, this->width, config);
			generator.generateIsland();
			generator.secondPass();
			generators.push_back(generator);
			std::erase(squares, selectedSquare);
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			std::uniform_int_distribution<int> randomSquareSelector(0, squares.size() - 1);
			
			int recursionSquare = randomSquareSelector(rd);
			std::cout << "selected: " << recursionSquare << std::endl;
			Square selectedSquare = squares[recursionSquare];
			

			IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(selectedSquare.end.y - selectedSquare.begin.y, selectedSquare.end.x - selectedSquare.begin.x);
			VectorIslandGenerator generator(nullptr, &this->vectorWorldMap, selectedSquare.begin.x, selectedSquare.end.x, selectedSquare.begin.y, selectedSquare.end.y, this->width, config);
			generator.generateIsland();
			generator.secondPass();
			generators.push_back(generator);
			std::erase(squares, selectedSquare);
		}
	}
	/*tbb::task_group g;
	for (auto it = generators.begin(); it != generators.end(); it++) {
		VectorIslandGenerator gen = (*it);
		g.run([&] {gen.generateIsland(); });
	}
	g.wait();

	tbb::task_group g2;
	for (auto it = generators.begin(); it != generators.end(); it++) {
		VectorIslandGenerator gen = (*it);
		g2.run([&] {gen.secondPass(); });
	}
	g2.wait();*/
	std::cout << generators.size() << std::endl;

	this->isChanged = true;
}
