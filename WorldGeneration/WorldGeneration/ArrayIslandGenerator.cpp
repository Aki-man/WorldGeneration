#include "ArrayIslandGenerator.h"

ArrayIslandGenerator::~ArrayIslandGenerator()
{
	this->startWidth = 0;
	this->endWidth = 0;
	this->startLength = 0;
	this->endLength = 0;
	//this->arrayWorldMap = nullptr;
}

void ArrayIslandGenerator::insert(Coordinate coord, char tile)
{
	long address = coord.y * this->totalWidth + coord.x;
	this->arrayWorldMap[address] = tile;
}

void ArrayIslandGenerator::replace(Coordinate coord, char tile)
{
	long address = coord.y * this->totalWidth + coord.x;
	this->arrayWorldMap[address] = tile;
}

char ArrayIslandGenerator::get(Coordinate coord)
{
	long address = coord.y * this->totalWidth + coord.x;
	if (address > this->totalSize || address < 0)
		return '/0';
	return this->arrayWorldMap[address];
}

bool ArrayIslandGenerator::contains(Coordinate coord)
{
	return this->get(coord) != '/0';
}
