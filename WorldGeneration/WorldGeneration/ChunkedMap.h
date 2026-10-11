#pragma once
#include <vector>
#include "Coordinate.h"
#include <unordered_map>
#include <string>

class ChunkedMap {
public:
	int width;
	int length;
	int chunkSize;
	std::string name;
	std::unordered_map<Coordinate, std::vector<char>*> chunks;
	ChunkedMap() : width(0), length(0),
		chunkSize(0), chunks(std::unordered_map<Coordinate, std::vector<char>*>()),
	name(""){};
	ChunkedMap(int width, int length, std::string name) : width(width), length(length),
		chunkSize(length / 4), chunks(std::unordered_map<Coordinate, std::vector<char>*>()),
	name(name){};
	Coordinate getChunkId(Coordinate coord);
	char get(Coordinate coord);
	void insert(Coordinate coord, char tile);
	bool contains(Coordinate coord);
	bool loadChunk(Coordinate chunkID);
};