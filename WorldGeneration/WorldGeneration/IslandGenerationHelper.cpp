#include "IslandGenerationHelper.h"
#include "IslandGeneratorConfiguration.h"
#include "VectorIslandGenerator.h"

void IslandGenerationHelper::SetUpGeneratorAndGenerate(std::vector<char>* map, Square selectedSquare, int totalWidth)
{
	IslandGeneratorConfiguration config = IslandGeneratorConfiguration::generateConfiguration(selectedSquare.end.y - selectedSquare.begin.y, selectedSquare.end.x - selectedSquare.begin.x);
	VectorIslandGenerator generator(nullptr, map, selectedSquare.begin.x, selectedSquare.end.x, selectedSquare.begin.y, selectedSquare.end.y, totalWidth, config);
	generator.generateIsland();
	generator.secondPass();
}
