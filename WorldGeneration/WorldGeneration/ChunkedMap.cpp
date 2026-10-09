#include "ChunkedMap.h"

Coordinate ChunkedMap::getChunkId(Coordinate coord)
{
    int chunkX = coord.x / this->chunkSize;
    int chunkY = coord.y / this->chunkSize;
    return Coordinate(chunkX, chunkY);
}

char ChunkedMap::get(Coordinate coord)
{
    Coordinate id = this->getChunkId(coord);
    int offsetX = coord.x - (id.x*this->chunkSize);
    int offsetY = coord.y - (id.y*this->chunkSize);
    return (*this->chunks.at(id))[offsetY * this->chunkSize + offsetX];
}

void ChunkedMap::insert(Coordinate coord, char tile)
{
    Coordinate id = this->getChunkId(coord);
    int offsetX = coord.x - (id.x * this->chunkSize);
    int offsetY = coord.y - (id.y * this->chunkSize);
    (*this->chunks.at(id))[offsetY * this->chunkSize + offsetX] = tile;
}

bool ChunkedMap::contains(Coordinate coord)
{
    Coordinate id = this->getChunkId(coord);
    return this->chunks.contains(id);
}
