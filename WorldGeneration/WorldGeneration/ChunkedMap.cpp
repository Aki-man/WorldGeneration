#include "ChunkedMap.h"
#include <filesystem>
#include <fstream>

Coordinate ChunkedMap::getChunkId(Coordinate coord)
{
    int chunkX = coord.x / this->chunkSize;
    int chunkY = coord.y / this->chunkSize;
    return Coordinate(chunkX, chunkY);
}

void ChunkedMap::increaseChunkUse()
{
	for (auto it = this->chunkUse.begin(); it != this->chunkUse.end(); ++it) {
		it->second++;
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
    int offsetX = coord.x - (id.x * this->chunkSize);
    int offsetY = coord.y - (id.y * this->chunkSize);
    this->chunks.at(id)[offsetY * this->chunkSize + offsetX] = tile;
}

bool ChunkedMap::contains(Coordinate coord)
{
    Coordinate id = this->getChunkId(coord);
	if (this->chunks.contains(id))
		return true;
	return this->loadChunk(id);
    //return this->chunks.contains(id);
}

bool ChunkedMap::loadChunk(Coordinate chunkID)
{
	std::string fileName = "data/saves/" + this->name + "/save" 
		+ std::to_string(chunkID.x) + std::to_string(chunkID.y) + ".txt";
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

void ChunkedMap::cleanUnusedChunks()
{
	std::vector<Coordinate> erasedChunks = std::vector<Coordinate>();
	for (auto it = this->chunkUse.begin(); it != this->chunkUse.end(); ++it) {
		if (it->second >= 10) {
			this->chunks.erase(it->first);
			erasedChunks.push_back(it->first);
		}
	}
	for (auto it = erasedChunks.begin(); it != erasedChunks.end(); ++it) this->chunkUse.erase(*it);
}

void ChunkedMap::unloadChunk(Coordinate chunkID)
{
	this->chunks.erase(chunkID);
}
