#include "TileColorMap.h"

std::string TileColorMap::get(char tile)
{
    if (this->tileColorMap.contains(tile)) {
        return this->tileColorMap.at(tile);
    }
    return "\x1b[0m";
}
