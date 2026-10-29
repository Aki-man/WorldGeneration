#pragma once
#include <vector>
#include "Coordinate.h"
#include "HashInjection.h"
#include <unordered_map>
#include <string>

class ChunkedMap {
public:
	int width;
	int length;
	int chunkSize;
	std::string name;
	std::unordered_map<Coordinate, std::vector<char>> chunks;
	std::unordered_map<Coordinate, int> chunkUse;
	ChunkedMap() : width(0), length(0),
		chunkSize(0), chunks(std::unordered_map<Coordinate, std::vector<char>>()),
	name(""), chunkUse(std::unordered_map<Coordinate, int>()) {};
	ChunkedMap(int width, int length, std::string name) : width(width), length(length),
		chunkSize(length / 4), chunks(std::unordered_map<Coordinate, std::vector<char>>()),
	name(name), chunkUse(std::unordered_map<Coordinate, int>()) {};
	Coordinate getChunkId(Coordinate coord);
	void increaseChunkUse();
	char get(Coordinate coord);
	void insert(Coordinate coord, char tile);
	bool contains(Coordinate coord);
	bool loadChunk(Coordinate chunkID);
	bool loadAllAdjacentChunks(Coordinate chunkID);
	bool load(Coordinate playerCoordinate);
	int calculateDistance(Coordinate currentCoordinate, Coordinate chunkID);
	void cleanUnusedChunks(Coordinate currentCoordinate);
	void unloadChunk(Coordinate chunkID);
};