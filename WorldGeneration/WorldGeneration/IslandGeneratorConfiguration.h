#pragma once
class IslandGeneratorConfiguration {
public:
	const int islandGenerationStart;
	const int mapSeaBorderSize;
	const int randomIslandWidthChange;
	const int randomIslandOffsetChange;
	const double percentageForIslandNarrowing;
	const int forestSpawnChance;
	const int mountainGenerationStart;
	const int randomMountainWidthChange ;
	const int randomMountainOffsetChange;
	IslandGeneratorConfiguration() : islandGenerationStart(5), 
		mapSeaBorderSize(6),
		randomIslandWidthChange(5),
		randomIslandOffsetChange(5),
		percentageForIslandNarrowing(0.25),
		forestSpawnChance(8),
		mountainGenerationStart(3),
		randomMountainWidthChange(2),
		randomMountainOffsetChange(3){};
	IslandGeneratorConfiguration(int islandGenerationStart, int mapSeaBorder, int randomIslandWidthChange, int randomIslandOffsetChange,
	double percentageForIslandNarrowing, int forestChance, int mountainGenerationStart,int randomMountainWidthChange,
	 int randomMountainOffsetChange) : islandGenerationStart(islandGenerationStart), mapSeaBorderSize(mapSeaBorder),
	randomIslandWidthChange(randomIslandWidthChange), randomIslandOffsetChange(randomIslandOffsetChange), 
		percentageForIslandNarrowing(percentageForIslandNarrowing), forestSpawnChance(forestChance), mountainGenerationStart(mountainGenerationStart),
		randomMountainWidthChange(randomMountainWidthChange),randomMountainOffsetChange(randomMountainOffsetChange){};
	
	~IslandGeneratorConfiguration();

};