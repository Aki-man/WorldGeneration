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
