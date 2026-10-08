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
	long address = coord.y * this->totalWidth + coord.x;
	(*this->vectorWorldMap)[address] = tile;
}

void VectorIslandGenerator::replace(Coordinate coord, char tile)
{
	long address = coord.y * this->totalWidth + coord.x;
	(*this->vectorWorldMap)[address] = tile;
}

char VectorIslandGenerator::get(Coordinate coord)
{
	long address = coord.y * this->totalWidth + coord.x;
	if (address > (*this->vectorWorldMap).size())
		return '=';
	return (*this->vectorWorldMap)[address];
}

bool VectorIslandGenerator::contains(Coordinate coord)
{
	//if (coord.y * this->totalWidth + coord.x > (*this->vectorWorldMap).size())
	//	return false;
	return this->get(coord) != '\0';
}
