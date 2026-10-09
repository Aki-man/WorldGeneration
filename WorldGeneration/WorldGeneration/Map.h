#pragma once
#include "coordinate.h"
#include "HashInjection.h"
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
#include <string>

class WorldMap {
protected:
	
	int width;
	int length;
	int chunkSize;
public:

	std::string name;
	std::unordered_map<Coordinate, char> worldMap;
	bool isChanged;
	WorldMap() : width(0), length(0), name(""), worldMap(std::unordered_map<Coordinate, char>()), isChanged(false), chunkSize(0) {};
	WorldMap(int width, int length) : width(width), length(length), name(""),worldMap(std::unordered_map<Coordinate, char>()), isChanged(false), chunkSize(length/4) {};
	~WorldMap();
	virtual void insert(Coordinate coord, char tile);
	virtual char get(Coordinate coord);
	virtual bool contains(Coordinate coord);
	virtual void GenerateOneIslandMap();
	virtual void GenerateFourIslandMap();
	void print(std::ostream& out);
	void save(std::string saveName);
	void parallelSave(std::string saveName);
	void saveMapChunk(std::string fileName, int i, int l);
	bool load(std::string saveName);
	virtual void loadMap(int fileNumber, std::string saveName);
	void loadMapChunk(std::string fileName, int i, int l);
	std::string getChunkId(Coordinate coord);
	friend std::ostream& operator<<(std::ostream& out, WorldMap& map);
	
};
