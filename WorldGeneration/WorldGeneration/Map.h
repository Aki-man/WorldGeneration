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
	
public:
	std::string name;
	std::unordered_map<Coordinate, char> worldMap;
	WorldMap() : width(0), length(0), name(""), worldMap(std::unordered_map<Coordinate, char>()) {};
	WorldMap(int width, int length) : width(width), length(length), name(""),worldMap(std::unordered_map<Coordinate, char>()) {};
	~WorldMap();
	virtual char get(Coordinate coord);
	virtual void GenerateOneIslandMap();
	virtual void GenerateFourIslandMap();
	void print(std::ostream& out);
	virtual void save(std::string saveName);
	void parallelSave(std::string saveName);
	void saveMapChunk(std::string fileName, int i, int l);
	virtual bool load(std::string saveName);
	friend std::ostream& operator<<(std::ostream& out, WorldMap& map);
	
};
