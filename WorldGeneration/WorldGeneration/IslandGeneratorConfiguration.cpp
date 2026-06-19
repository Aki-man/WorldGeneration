#include "IslandGeneratorConfiguration.h"

IslandGeneratorConfiguration IslandGeneratorConfiguration::generateConfiguration(int islandLength, int islandWidth)
{
	return IslandGeneratorConfiguration(
	int(islandLength/20),
	int(islandLength / 20),
		int(islandLength / 20),
		int(islandLength / 20),
		0.25,
		8,
		int(islandLength / 25),
		int(islandLength / 25),
		int(islandLength / 25),
		int(islandLength / 15)
	);
}
