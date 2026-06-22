#include "ParallelIslandGenerator.h"
#include "ParallelSecondPassHelper.h"

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
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
			bool found = (*this->parallelWorldMap).find(a, coord);
			
			char tile = '=';
			if(found)
				tile = a->second;
			a.release();
			if (this->isAdjacentTo(coord, '~') && tile != '~' && tile != 'R' && tile != 'M') {
				tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor writer;
				(*this->parallelWorldMap).insert(writer, coord);
				writer->second = 'C';
				writer.release();
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

void ParallelIslandGenerator::parallelSecondPass()
{
	int lakeNumber = config.lakeNumber;
	bool riverGenerated = false;
	tbb::parallel_for(tbb::blocked_range<size_t>(startLength, endLength),
		ParallelSecondPassHelper(this, this->parallelWorldMap, lakeNumber, &riverGenerated, this->startLength, this->endLength),
		tbb::auto_partitioner());
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
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
	(*this->parallelWorldMap).insert(a, coord);
	a->second = tile;
	/*std::uniform_int_distribution<int> randomCoordinateSelector(0, coordinatesToConvert.size() - 1);
	for (int i = 0; i < clumpSize; ++i) {
		Coordinate coord = coordinatesToConvert[randomCoordinateSelector(rd)];
		bool found = (*this->parallelWorldMap).find(a, coord);
		if (found) {
			char foundTile = a->second;
			if (foundTile != 'C' && foundTile != '~' && foundTile != 'R' && foundTile != 'M') {
				(*this->parallelWorldMap).insert(a, coord);
				a->second = tile;
			}
		}
	}*/
	this->replaceRandomTiles(coordinatesToConvert, clumpSize, tile);
}

void ParallelIslandGenerator::replaceRandomTiles(std::vector<Coordinate> coordinatesToConvert, int clumpSize, char tile) {
	std::uniform_int_distribution<int> randomCoordinateSelector(0, coordinatesToConvert.size() - 1);
	for (int i = 0; i < clumpSize; ++i) {
		Coordinate coord = coordinatesToConvert[randomCoordinateSelector(rd)];
		tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
		bool found = (*this->parallelWorldMap).find(a, coord);
		if (found) {
			char foundTile = a->second;
			if (foundTile != 'C' && foundTile != '~' && foundTile != 'R' && foundTile != 'M') {
				(*this->parallelWorldMap).insert(a, coord);
				a->second = tile;
			}
		}
	}
}

void ParallelIslandGenerator::generateRiver(Coordinate startCoordinate, bool isGoingLeft, bool isGoingUp)
{
	Coordinate currentCoordinate = startCoordinate;
	while (true) {
		tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
		(*this->parallelWorldMap).insert(a, currentCoordinate);
		a->second = 'R';
		a.release();
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
		tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
		bool found = (*this->parallelWorldMap).find(a, coord);
		if (found) {
			char foundTile = a->second;
			if (foundTile == tile) {
				return true;
			}
		}
	}
	return false;
}

void ParallelIslandGenerator::generateSeaLine(int length)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		(*this->parallelWorldMap).insert(a, coord);
		a->second = '~';
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
				tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
				(*this->parallelWorldMap).insert(a, coord);
				a->second = 'T';
			}
			else {
				tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
				(*this->parallelWorldMap).insert(a, coord);
				a->second = 'O';
			}
		}
		else {
			Coordinate coord(i, length);
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			(*this->parallelWorldMap).insert(a, coord);
			a->second = '~';
			
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
					tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					(*this->parallelWorldMap).insert(a, coord);
					a->second = 'T';
				}
				else {
					tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					(*this->parallelWorldMap).insert(a, coord);
					a->second = 'M';
				}
				continue;
			}
			else {
				int forest = forestChanceGenerator(rd);
				if (forest == config.forestSpawnChance) {
					tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					(*this->parallelWorldMap).insert(a, coord);
					a->second = 'T';
				}
				else {
					tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					(*this->parallelWorldMap).insert(a, coord);
					a->second = 'O';
				}
			}
		}
		else {
			Coordinate coord(i, length);
			tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			(*this->parallelWorldMap).insert(a, coord);
			a->second = '~';
		}
	}
}
