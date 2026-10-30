#include "ChunkedMap.h"
#include <filesystem>
#include <fstream>
#include <cmath>

Coordinate ChunkedMap::getChunkId(Coordinate coord)
{
    int chunkX = coord.x / this->chunkSize;
    int chunkY = coord.y / this->chunkSize;
    return Coordinate(chunkX, chunkY);
}

void ChunkedMap::increaseChunkUse()
{
	for (auto it = this->chunkUse.begin(); it != this->chunkUse.end(); ++it) {
		if(it->second < 10)it->second++;
	}
}

char ChunkedMap::get(Coordinate coord)
{
    Coordinate id = this->getChunkId(coord);
	this->chunkUse[id] = 0;
    int offsetX = coord.x - (id.x*this->chunkSize);
    int offsetY = coord.y - (id.y*this->chunkSize);
    return this->chunks.at(id)[offsetY * this->chunkSize + offsetX];
}

void ChunkedMap::insert(Coordinate coord, char tile)
{
    Coordinate id = this->getChunkId(coord);
	this->chunkUse[id] = 0;
    int offsetX = coord.x - (id.x * this->chunkSize);
    int offsetY = coord.y - (id.y * this->chunkSize);
    this->chunks.at(id)[offsetY * this->chunkSize + offsetX] = tile;
}

bool ChunkedMap::contains(Coordinate coord)
{
	if (coord.x < 0 || coord.x > this->width || coord.y < 0 || coord.y > this->length)
		return false;
    Coordinate id = this->getChunkId(coord);
	if (this->chunks.contains(id))
		return true;
	return this->loadChunk(id);
    //return this->chunks.contains(id);
}

bool ChunkedMap::loadChunk(Coordinate chunkID)
{
	std::string fileName = "data/saves/" + this->name + "/save" 
		+ std::to_string(chunkID.x) + "_" + std::to_string(chunkID.y) + ".txt";
	if (std::filesystem::exists(fileName) == false)
		return false;
	std::ifstream file(fileName, std::ios::out | std::ios::binary);
	int chunkNumber = this->length / this->chunkSize;
	char currentTile = '=';
	int currentCount = 0;
	file >> currentCount;
	file >> currentTile;
	std::vector<char> chunk(this->chunkSize*this->chunkSize);
	for (int j = 0; j < this->chunkSize; ++j) {
		for (int k = 0; k < this->chunkSize; ++k) {
			if (currentCount != 0) {
				//this->insert(Coordinate(k, j), currentTile);
				chunk[j * this->chunkSize + k] = currentTile;
				currentCount--;
			}
			else {
				file >> currentCount;
				file >> currentTile;
				//this->insert(Coordinate(k, j), currentTile);
				chunk[j * this->chunkSize + k] = currentTile;
				currentCount--;
			}

		}
	}
	this->chunks[chunkID] = chunk;
	this->chunkUse[chunkID] = 0;
	file.close();
	return true;
}

bool ChunkedMap::loadAllAdjacentChunks(Coordinate chunkID)
{
	std::vector<Coordinate> coords = { chunkID,
	Coordinate(chunkID.x - 1, chunkID.y),
	Coordinate(chunkID.x + 1, chunkID.y),
	Coordinate(chunkID.x, chunkID.y - 1),
	Coordinate(chunkID.x, chunkID.y + 1),
	Coordinate(chunkID.x - 1, chunkID.y - 1),
	Coordinate(chunkID.x - 1, chunkID.y + 1),
	Coordinate(chunkID.x + 1, chunkID.y - 1),
	Coordinate(chunkID.x + 1, chunkID.y + 1) };
	for (int i = 0; i < coords.size(); ++i) {
		this->loadChunk(coords[i]);
	}
}

bool ChunkedMap::load(Coordinate playerCoordinate)
{
	std::string fileName = "data/saves/" + this->name + "/save"
		+ std::to_string(0)  + ".txt";
	if (std::filesystem::exists(fileName) == false)
		return false;
	std::ifstream file(fileName, std::ios::out | std::ios::binary);
	file >> this->width >> this->length >> this->chunkSize;
	this->loadAllAdjacentChunks(this->getChunkId(playerCoordinate));
}

int ChunkedMap::calculateDistance(Coordinate currentCoordinate, Coordinate chunkID)
{
	int dx = std::max((chunkID.x * this->chunkSize) - currentCoordinate.x, currentCoordinate.x - ((chunkID.x + 1) * this->chunkSize));
	int dy = std::max((chunkID.y * this->chunkSize) - currentCoordinate.y, currentCoordinate.y - ((chunkID.y + 1) * this->chunkSize));
	return std::sqrt(pow(dx, 2) + pow(dy, 2));
}

void ChunkedMap::cleanUnusedChunks(Coordinate currentCoordinate)
{
	std::vector<Coordinate> erasedChunks = std::vector<Coordinate>();
	for (auto it = this->chunkUse.begin(); it != this->chunkUse.end(); ++it) {
		if (it->second >= 10) {
			
			//int dx = std::max((it->first.x*this->chunkSize) - currentCoordinate.x,currentCoordinate.x - ((it->first.x+1) * this->chunkSize));
			//int dy = std::max((it->first.y*this->chunkSize) - currentCoordinate.y, currentCoordinate.y - ((it->first.y+1) * this->chunkSize));
			//int distance = std::sqrt(pow(dx, 2) + pow(dy, 2));
			int distance = this->calculateDistance(currentCoordinate, it->first);
			if (distance > this->chunkSize) {
				this->chunks.erase(it->first);
				erasedChunks.push_back(it->first);
			}
		}
	}
	for (auto it = erasedChunks.begin(); it != erasedChunks.end(); ++it) this->chunkUse.erase(*it);
}

void ChunkedMap::unloadChunk(Coordinate chunkID)
{
	this->chunks.erase(chunkID);
}
