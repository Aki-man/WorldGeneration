#include "VectorIslandGenerator.h"

VectorIslandGenerator::~VectorIslandGenerator()
{
	this->startWidth = 0;
	this->endWidth = 0;
	this->startLength = 0;
	this->endLength = 0;
}

void VectorIslandGenerator::insert(Coordinate coord, char tile)
{
	(*this->vectorWorldMap)[coord.y * this->totalWidth + coord.x] = tile;
}

void VectorIslandGenerator::replace(Coordinate coord, char tile)
{
	(*this->vectorWorldMap)[coord.y * this->totalWidth + coord.x] = tile;
}

char VectorIslandGenerator::get(Coordinate coord)
{
	return (*this->vectorWorldMap)[coord.y * this->totalWidth + coord.x];
}

bool VectorIslandGenerator::contains(Coordinate coord)
{
	return this->get(coord) != '\0';
}
