#pragma once
class IslandGeneratorConfiguration {
public:
	const int islandGenerationStart = 1;
	const int mapSeaBorderSize = 2;
	const int randomIslandWidthChange = 2;
	const int randomIslandOffsetChange = 3;
	const double percentageForIslandNarrowing = 0.25;
	const int forestSpawnChance = 3;
	IslandGeneratorConfiguration(int islandGenerationStart, int mapSeaBorder, int randomIslandWidthChange, int randomIslandOffsetChange,
	double percentageForIslandNarrowing, int forestChance) : islandGenerationStart(islandGenerationStart), mapSeaBorderSize(mapSeaBorder),
	randomIslandWidthChange(randomIslandWidthChange), randomIslandOffsetChange(randomIslandOffsetChange), 
		percentageForIslandNarrowing(percentageForIslandNarrowing), forestSpawnChance(forestChance){};
	~IslandGeneratorConfiguration();

};