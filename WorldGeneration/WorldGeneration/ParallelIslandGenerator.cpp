#include "ParallelIslandGenerator.h"
#include "ParallelSecondPassHelper.h"

ParallelIslandGenerator::~ParallelIslandGenerator()
{
	
	this->startWidth = 0;
	this->endWidth = 0;
	this->startLength = 0;
	this->endLength = 0;
}

void ParallelIslandGenerator::insert(Coordinate coord, char tile)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
	(*this->parallelWorldMap).insert(a, coord);
	a->second = tile;
}

void ParallelIslandGenerator::replace(Coordinate coord, char tile)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
	(*this->parallelWorldMap).insert(a, coord);
	a->second = tile;
}

char ParallelIslandGenerator::get(Coordinate coord)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
	bool found = (*this->parallelWorldMap).find(a, coord);

	char tile = '=';
	if (found)
		tile = a->second;
	return tile;
}

bool ParallelIslandGenerator::contains(Coordinate coord)
{
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::const_accessor a;
	return (*this->parallelWorldMap).find(a, coord);
}

void ParallelIslandGenerator::parallelSecondPass()
{
	int lakeNumber = config.lakeNumber;
	bool riverGenerated = false;
	tbb::parallel_for(tbb::blocked_range<size_t>(startLength, endLength),
		ParallelSecondPassHelper(this, this->parallelWorldMap, &lakeNumber, &riverGenerated, this->startLength, this->endLength),
		tbb::auto_partitioner());
}

void ParallelIslandGenerator::generateSeaLine(int length)
{
	//tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
	for (int i = startWidth; i < endWidth; ++i) {
		Coordinate coord(i, length);
		/*(*this->parallelWorldMap).insert(a, coord);
		a->second = '~';*/
		this->insert(coord, '~');
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
				/*tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
				(*this->parallelWorldMap).insert(a, coord);
				a->second = 'T';*/
				this->insert(coord, 'T');
			}
			else {
				/*tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
				(*this->parallelWorldMap).insert(a, coord);
				a->second = 'O';*/
				this->insert(coord, 'O');
			}
		}
		else {
			Coordinate coord(i, length);
			/*tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			(*this->parallelWorldMap).insert(a, coord);
			a->second = '~';*/
			this->insert(coord, '~');
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
					/*tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					(*this->parallelWorldMap).insert(a, coord);
					a->second = 'T';*/
					this->insert(coord, 'T');
				}
				else {
					/*tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					(*this->parallelWorldMap).insert(a, coord);
					a->second = 'M';*/
					this->insert(coord, 'M');
				}
				continue;
			}
			else {
				int forest = forestChanceGenerator(rd);
				if (forest == config.forestSpawnChance) {
					//tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					//(*this->parallelWorldMap).insert(a, coord);
					//a->second = 'T';
					this->insert(coord, 'T');
				}
				else {
					//tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
					//(*this->parallelWorldMap).insert(a, coord);
					//a->second = 'O';
					this->insert(coord, 'O');
				}
			}
		}
		else {
			Coordinate coord(i, length);
			/*tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>::accessor a;
			(*this->parallelWorldMap).insert(a, coord);
			a->second = '~';*/
			this->insert(coord, '~');
		}
	}
}
