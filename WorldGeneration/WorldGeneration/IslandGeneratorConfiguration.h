#pragma once
class IslandGeneratorConfiguration {
public:
	const int islandGenerationStart = 5;
	const int mapSeaBorderSize = 2;
	const int randomIslandWidthChange = 2;
	const int randomIslandOffsetChange = 3;
	const double percentageForIslandNarrowing = 0.25;
	const int forestSpawnChance = 3;
	const int mountainGenerationStart = 3;
	const int randomMountainWidthChange = 2;
	const int randomMountainOffsetChange = 3;
	IslandGeneratorConfiguration() : islandGenerationStart(1), mapSeaBorderSize(2),randomIslandWidthChange(2), randomIslandOffsetChange(3),
		percentageForIslandNarrowing(0.25), forestSpawnChance(3), mountainGenerationStart(3), randomMountainWidthChange(2), randomMountainOffsetChange(3){};
	IslandGeneratorConfiguration(int islandGenerationStart, int mapSeaBorder, int randomIslandWidthChange, int randomIslandOffsetChange,
	double percentageForIslandNarrowing, int forestChance, int mountainGenerationStart,int randomMountainWidthChange,
	 int randomMountainOffsetChange) : islandGenerationStart(islandGenerationStart), mapSeaBorderSize(mapSeaBorder),
	randomIslandWidthChange(randomIslandWidthChange), randomIslandOffsetChange(randomIslandOffsetChange), 
		percentageForIslandNarrowing(percentageForIslandNarrowing), forestSpawnChance(forestChance), mountainGenerationStart(mountainGenerationStart),
		randomMountainWidthChange(randomMountainWidthChange),randomMountainOffsetChange(randomMountainOffsetChange){};
	
	~IslandGeneratorConfiguration();

};