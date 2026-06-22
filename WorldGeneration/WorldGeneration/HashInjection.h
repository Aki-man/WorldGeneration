#pragma once
#include <cmath>
#include "Coordinate.h"

template<>
struct std::hash<Coordinate>
{
    std::size_t operator()(const Coordinate& coord) const noexcept
    {
        return coord.x * 100000 + coord.y;
    }
};