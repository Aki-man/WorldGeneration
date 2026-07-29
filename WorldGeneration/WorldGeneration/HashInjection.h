#pragma once
#include <cmath>
#include "Coordinate.h"

template<>
struct std::hash<Coordinate>
{
    std::size_t operator()(const Coordinate& coord) const noexcept
    {
		size_t h = 0;
		h += static_cast<unsigned long long>(coord.x) * 100000 + coord.y;
		return h;
    }
};