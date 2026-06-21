#include "ParallelIslandGenerator.h"
#include <concurrent_unordered_map.h>

ParallelIslandGenerator::~ParallelIslandGenerator()
{
	
	this->startWidth = 0;
	this->endWidth = 0;
	this->startLength = 0;
	this->endLength = 0;
}

void ParallelIslandGenerator::secondPass()
{
	int lakeNumber = config.lakeNumber;
	bool riverGenerated = false;
	for (int i = startLength; i < endLength; ++i) {
		for (int j = startWidth; j < endWidth; ++j) {
			Coordinate coord(i, j);
			
			bool found = (*this->parallelWorldMap).contains(coord);
			
			char tile = '=';
			if(found)
				tile = (*this->parallelWorldMap).at(coord);
			if (this->isAdjacentTo(coord, '~') && tile != '~' && tile != 'R' && tile != 'M') {
				
				(*this->parallelWorldMap).insert({ coord, 'C' });
				
			}
			else if ((tile == 'O' ||tile == 'T') && lakeNumber > 0) {
				std::uniform_int_distribution<int> randomLakesize(1, 8);
				if (std::rand() % 20 == 0) {
					this->generateTileClump(coord, randomLakesize(rd), 'L');
					--lakeNumber;
				}
			}
			if (tile == 'T') {
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

void ParallelIslandGenerator::generateTileClump(Coordinate coord, int clumpSize, char tile)
{
	std::vector<Coordinate> coordinatesToConvert = { Coordinate(coord.x, coord.y - 1),
	Coordinate(coord.x, coord.y + 1),
	Coordinate(coord.x - 1, coord.y),
	Coordinate(coord.x + 1, coord.y - 1),
	Coordinate(coord.x - 1, coord.y - 1),
	Coordinate(coord.x - 1, coord.y + 1),
	Coordinate(coord.x + 1, coord.y - 1),
	Coordinate(coord.x + 1, coord.y + 1) };
	(*this->parallelWorldMap).insert({ coord, tile });
	this->replaceRandomTiles(coordinatesToConvert, clumpSize, tile);
}

void ParallelIslandGenerator::replaceRandomTiles(std::vector<Coordinate> coordinatesToConvert, int clumpSize, char tile) {
	std::uniform_int_distribution<int> randomCoordinateSelector(0, coordinatesToConvert.size() - 1);
	for (int i = 0; i < clumpSize; ++i) {
		Coordinate coord = coordinatesToConvert[randomCoordinateSelector(rd)];
		bool found = (*this->parallelWorldMap).contains(coord);
		if (found) {
			char foundTile = (*this->parallelWorldMap).at(coord);
			if (foundTile != 'C' && foundTile != '~' && foundTile != 'R' && foundTile != 'M') {
				(*this->parallelWorldMap).insert({ coord, tile });
			}
		}
	}
}

void ParallelIslandGenerator::generateRiver(Coordinate startCoordinate, bool isGoingLeft, bool isGoingUp)
{
	Coordinate currentCoordinate = startCoordinate;
	while (true) {
		(*this->parallelWorldMap).insert({ currentCoordinate, 'R' });
		
		
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

bool ParallelIslandGenerator::isAdjacentTo(Coordinate coord, char tile)
{
	Coordinate leftAdjacentTile = Coordinate(coord.x - 1, coord.y);
	Coordinate rightAdjacentTile = Coordinate(coord.x + 1, coord.y);
	Coordinate upAdjacentTile = Coordinate(coord.x, coord.y + 1);
	Coordinate downAdjacentTile = Coordinate(coord.x, coord.y - 1);
	std::vector<Coordinate> coordinatesToCheck = { leftAdjacentTile, rightAdjacentTile, upAdjacentTile, downAdjacentTile };
	for (Coordinate coord : coordinatesToCheck)
	{
		bool found = (*this->parallelWorldMap).contains(coord);
		if (found) {
			char foundTile = (*this->parallelWorldMap).at(coord);
			if (foundTile == tile) {
				return true;
			}
		}
	}
	return false;
}

void ParallelIslandGenerator::generateSeaLine(int length)
{
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		(*this->parallelWorldMap).insert({ coord , '~'});
	}
}

void ParallelIslandGenerator::generateIslandLine(int island_begin, int island_end, int length)
{
	std::uniform_int_distribution<int> forestChanceGenerator(0, config.forestSpawnChance);
	
	for (int i = startWidth; i < endWidth; ++i) {
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);
			int forest = forestChanceGenerator(rd);
			if (forest == config.forestSpawnChance) {

				(*this->parallelWorldMap).insert({ coord, 'T'});
			}
			else {
				(*this->parallelWorldMap).insert({ coord, 'O' });
				
			}
		}
		else {
			Coordinate coord(i, length);
			(*this->parallelWorldMap).insert({ coord, '~' });
			
		}
	}
}

void ParallelIslandGenerator::generateIslandLineWithMountain(int island_begin, int island_end, int length, int mountain_begin, int mountain_end)
{
	std::uniform_int_distribution<int> forestChanceGenerator(0, config.forestSpawnChance);
	std::uniform_int_distribution<int> mountainForestChanceGenerator(0, config.forestSpawnChance + (config.forestSpawnChance * 0.25));
	
	for (int i = startWidth; i < endWidth; ++i) {
		if (i >= island_begin && i <= island_end) {
			Coordinate coord(i, length);
			if (i > mountain_begin && i <= mountain_end) {
				int forest = mountainForestChanceGenerator(rd);
				if (forest == config.forestSpawnChance) {
					(*this->parallelWorldMap).insert({ coord, 'T'});
				}
				else {
					(*this->parallelWorldMap).insert({ coord, 'M'});
				}
				continue;
			}
			else {
				int forest = forestChanceGenerator(rd);
				if (forest == config.forestSpawnChance) {
					(*this->parallelWorldMap).insert({ coord, 'T' });
				}
				else {
					(*this->parallelWorldMap).insert({ coord, 'O'});
				}
			}
		}
		else {
			Coordinate coord(i, length);
			(*this->parallelWorldMap).insert({ coord, '~'});
			
		}
	}
}
