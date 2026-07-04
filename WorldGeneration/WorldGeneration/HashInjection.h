#pragma once
#include <cmath>
#include "Coordinate.h"

template<>
struct std::hash<Coordinate>
{
    std::size_t operator()(const Coordinate& coord) const noexcept
    {
		size_t h = 0;
		//h += key.x * 3 + key.y * 2 + std::pow(key.x, key.y);
		//std::string hash = std::to_string(key.x) + std::to_string(key.y);
		/*std::hash<string> string_hasher; */
		h += static_cast<unsigned long long>(coord.x) * 100000 + coord.y;
		return h;
    }
};