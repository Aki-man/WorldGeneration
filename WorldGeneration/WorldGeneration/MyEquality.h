#pragma once
#include "Coordinate.h"

struct MyEquality {
    bool operator()(const Coordinate& coord1, const Coordinate& coord2) const {
        return (coord1.x == coord2.x && coord1.y == coord2.y);
     }
};