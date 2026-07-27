#include "IslandGenerator.h"
#include "IslandGeneratorConfiguration.h"
#include <random>
#include "LengthAndStartGenerator.h"

IslandGenerator::~IslandGenerator()
{
	this->worldMap = nullptr;
	this->startWidth = 0;
	this->endWidth = 0;
	this->startLength = 0;
	this->endLength = 0;
}

void IslandGenerator::generateIsland()
{
	std::uniform_int_distribution<int>  startGenerator(0, endWidth - startWidth);
	std::uniform_int_distribution<int>  lengthGenerator(int((endWidth-startWidth)/6), int((endWidth - startWidth) / 4));
	std::uniform_int_distribution<int> mountainGenerationStartGenerator(startLength, endLength);
	
	
	int mountainStartOfGeneration = mountainGenerationStartGenerator(rd);
	std::uniform_int_distribution<int> mountainGenerationEndGenerator(mountainStartOfGeneration, endLength);
	int mountainEndOfGeneration = mountainGenerationEndGenerator(rd);
	int islandLength = lengthGenerator(rd);
	int islandStart = startGenerator(rd);

	std::uniform_int_distribution<int> islandEndGenerator(endLength - config.islandGenerationStart - 3, endLength - 3);
	int islandEnd = islandEndGenerator(rd);

	int mountainStart = 0;
	int mountainLength = 0;
	bool generateIsland = false;
	bool generateMountain = false;
	bool getWider = false;
	LengthAndStartGenerator lengthAndStartGen(this->startWidth, this->endWidth, this->rd, this->config);
	for (int i = startLength; i < endLength; ++i) {
		
		std::tuple lengthAndStart = lengthAndStartGen.generateIslandLengthAndStart(islandLength, islandStart, i, getWider);
		islandLength = std::get<0>(lengthAndStart);
		islandStart = std::get<1>(lengthAndStart);

		if (islandLength < (endWidth - startWidth)/4 && i < config.percentageForIslandNarrowing * endLength)
			getWider = true;
		else if (i > config.percentageForIslandNarrowing * endLength)
			getWider = false;

		std::tuple mountainLengthAndStart = lengthAndStartGen.generateMountainLengthAndStart(lengthAndStart, mountainLength, mountainStart);
		mountainLength = std::get<0>(mountainLengthAndStart);
		mountainStart = std::get<1>(mountainLengthAndStart);

		this->generateIslandOrIslandWithMountain(i, islandLength, islandStart, mountainLength, mountainStart, generateIsland, generateMountain, mountainEndOfGeneration);

		generateIsland = this->shouldIslandGenerate(i, generateIsland, islandEnd);
	
		if (i > mountainEndOfGeneration)
			generateMountain = false;

		if (i > mountainStartOfGeneration && i < mountainEndOfGeneration && !generateMountain) {
			if (std::rand() % 4 == 0) {
				generateMountain = true;
				std::uniform_int_distribution<int> mountainStartGenerator(islandStart, (islandStart + islandLength));

				mountainStart = mountainStartGenerator(rd);

				std::uniform_int_distribution<int> mountainLengthGenerator(0, islandLength / 2);
				mountainLength = mountainLengthGenerator(rd);
			}
		}
	}
}

void IslandGenerator::generateIslandOrIslandWithMountain(int i, int islandLength, int islandStart, int mountainLength, int mountainStart,bool generateIsland, bool generateMountain, bool mountainEndOfGeneration)
{
	if (!generateIsland) {
		this->generateSeaLine(i);
	}
	else {
		if (!generateMountain)
			this->generateIslandLine(islandStart, islandStart + islandLength, i);
		else {
			this->generateIslandLineWithMountain(islandStart, islandStart + islandLength, i, mountainStart, mountainStart + mountainLength);
		}
	}
}



bool IslandGenerator::shouldIslandGenerate(int length, bool generateIsland, int islandEnd)
{
	if (length > config.islandGenerationStart && !generateIsland) {
		if (std::rand() % 4 == 0)
			generateIsland = true;
	}

	if (length > islandEnd)
		generateIsland = false;

	return generateIsland;
}

void IslandGenerator::insert(Coordinate coord, char tile)
{
	(*this->worldMap).insert({ coord, tile });
}

void IslandGenerator::replace(Coordinate coord, char tile)
{
	(*this->worldMap)[coord] = tile;
}

char IslandGenerator::get(Coordinate coord)
{
	return (*this->worldMap)[coord];
}

void IslandGenerator::secondPass() {
	int lakeNumber = config.lakeNumber;
	bool riverGenerated = false;
	for (int i = startLength; i < endLength; ++i) {
		for (int j = startWidth; j < endWidth; ++j) {
			Coordinate coord(i, j);
			if (this->isAdjacentTo(coord, '~') && (*this->worldMap)[coord] != '~' && (*this->worldMap)[coord] != 'R' && (*this->worldMap)[coord] != 'M') {
				//(*this->worldMap)[coord] = 'C';
				this->replace(coord, 'C');
			}
			else if (((*this->worldMap)[coord] == 'O' || (*this->worldMap)[coord] == 'T') && lakeNumber > 0) {
				std::uniform_int_distribution<int> randomLakesize(1, 8);
				if (std::rand() % 20 == 0) {
					this->generateTileClump(coord, randomLakesize(rd), 'L');
					--lakeNumber;
				}
			}
			if ((*this->worldMap)[coord] == 'T') {
				if (std::rand() % 3 == 0) {
					std::uniform_int_distribution<int> randomForestSize(0, 4);
					this->generateTileClump(coord, randomForestSize(rd), 'T');
				}
			}
			if (this->isAdjacentTo(coord, 'M') && !riverGenerated) {
				bool goingLeft = true;
				if (std::rand() % 2 == 0)
					goingLeft = false;
				bool goingUp = true;
				if (std::rand() % 2 == 0)
					goingUp = false;
				if (std::rand() % 10 == 0) {
					this->generateRiver(coord, goingLeft, goingUp);
					riverGenerated = true;
				}
			}
		}
	}
}

void IslandGenerator::generateTileClump(Coordinate coord, int clumpSize, char tile) {
	
	std::vector<Coordinate> coordinatesToConvert = { Coordinate(coord.x, coord.y - 1),
	Coordinate(coord.x, coord.y + 1),
	Coordinate(coord.x - 1, coord.y),
	Coordinate(coord.x + 1, coord.y - 1),
	Coordinate(coord.x - 1, coord.y - 1),
	Coordinate(coord.x - 1, coord.y + 1),
	Coordinate(coord.x + 1, coord.y - 1),
	Coordinate(coord.x + 1, coord.y + 1) };
	//(*this->worldMap)[coord] = tile;
	this->replace(coord, tile);
	std::uniform_int_distribution<int> randomCoordinateSelector(0, coordinatesToConvert.size()-1);
	for (int i = 0; i < clumpSize; ++i) {
		Coordinate coord = coordinatesToConvert[randomCoordinateSelector(rd)];
		if ((*this->worldMap).contains(coord)) {
			//char foundTile = (*worldMap)[coord];
			char foundTile = this->get(coord);
			if (foundTile != 'C' && foundTile != '~' && foundTile != 'R' && foundTile != 'M') {
				//(*this->worldMap)[coord] = tile;
				this->replace(coord, tile);
			}
		}
	}
}

void IslandGenerator::generateRiver(Coordinate startCoordinate, bool isGoingLeft, bool isGoingUp)
{
	Coordinate currentCoordinate = startCoordinate;
	while (true) {
		//(*this->worldMap)[currentCoordinate] = 'R';
		this->replace(currentCoordinate, 'R');
		if (this->isAdjacentTo(currentCoordinate, '~'))
			break;
		if (std::rand() % 2 == 0) {
			if (isGoingLeft)
				currentCoordinate = Coordinate(currentCoordinate.x - 1, currentCoordinate.y);
			else
				currentCoordinate = Coordinate(currentCoordinate.x + 1, currentCoordinate.y);
		}
		else {
			if (isGoingUp)
				currentCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y - 1);
			else
				currentCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y + 1);
		}
	}
}

bool IslandGenerator::isAdjacentTo(Coordinate coord, char tile) {

	Coordinate leftAdjacentTile = Coordinate(coord.x - 1, coord.y);
	Coordinate rightAdjacentTile = Coordinate(coord.x + 1, coord.y);
	Coordinate upAdjacentTile = Coordinate(coord.x, coord.y + 1);
	Coordinate downAdjacentTile = Coordinate(coord.x, coord.y - 1);
	std::vector<Coordinate> coordinatesToCheck = { leftAdjacentTile, rightAdjacentTile, upAdjacentTile, downAdjacentTile };
	for (Coordinate coord : coordinatesToCheck)
	{
		if ((*this->worldMap).contains(coord)) {
			//char foundTile = (*worldMap)[coord];
			char foundTile = this->get(coord);
			if (foundTile == tile) {
				return true;
			}
		}
	}
	return false;
}

void IslandGenerator::generateSeaLine(int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		//(*this->worldMap).insert({ coord, '~' });
		this->insert(coord, '~');
	}
}

void IslandGenerator::generateIslandLine(int island_begin, int island_end, int length)
{
	std::uniform_int_distribution<int> forestChanceGenerator(0, config.forestSpawnChance);
	for (int i = startWidth; i < endWidth; ++i) {
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);
			int forest = forestChanceGenerator(rd);
			if (forest == config.forestSpawnChance)
				//(*this->worldMap).insert({ coord,'T' });
				this->insert(coord, 'T');
			else
				//(*this->worldMap).emplace( coord, 'O' );
				this->insert(coord, 'O');
		}
		else {
			Coordinate coord(i, length);
			//(*this->worldMap).insert({ coord,'~' });
			this->insert(coord, '~');
		}
	}
}

void IslandGenerator::generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end)
{
	std::uniform_int_distribution<int> forestChanceGenerator(0, config.forestSpawnChance);
	std::uniform_int_distribution<int> mountainForestChanceGenerator(0, config.forestSpawnChance + (config.forestSpawnChance*0.25));
	for (int i = startWidth; i < endWidth; ++i) {
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);
			if (i > mountain_begin && i <= mountain_end) {
				int forest = mountainForestChanceGenerator(rd);
				if (forest == config.forestSpawnChance)
					//(*this->worldMap).insert({ coord, 'T' });
					this->insert(coord, 'T');
				else
					//(*this->worldMap).insert({ coord, 'M' });
					this->insert(coord, 'M');
				continue;
			}
			int forest = forestChanceGenerator(rd);
			if (forest == config.forestSpawnChance)
				//(*this->worldMap).insert({ coord,'T' });
				this->insert(coord, 'T');
			else
				//(*this->worldMap).insert({ coord,'O' });
				this->insert(coord, 'O');
		}
		else {
			Coordinate coord(i, length);
			//(*this->worldMap).insert({ coord,'~' });
			this->insert(coord, '~');
		}
	}
}
