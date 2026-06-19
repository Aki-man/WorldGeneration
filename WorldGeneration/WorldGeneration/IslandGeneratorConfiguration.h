#pragma once
class IslandGeneratorConfiguration {
public:
	const int islandGenerationStart = 1;
	const int mapSeaBorderSize = 2;
	const int randomIslandWidthChange = 2;
	const int randomIslandOffsetChange = 3;
	const double percentageForIslandNarrowing = 0.25;
	IslandGeneratorConfiguration(int islandGenerationStart, int mapSeaBorder, int randomIslandWidthChange, int randomIslandOffsetChange,
	double percentageForIslandNarrowing) : islandGenerationStart(islandGenerationStart), mapSeaBorderSize(mapSeaBorder),
	randomIslandWidthChange(randomIslandWidthChange), randomIslandOffsetChange(randomIslandOffsetChange), 
		percentageForIslandNarrowing(percentageForIslandNarrowing){};
	~IslandGeneratorConfiguration();

};