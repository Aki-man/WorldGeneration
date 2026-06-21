#pragma once
#include "Coordinate.h"
#include <cstddef>
#include <string>
#include <cmath>

struct MyHasher {
    std::size_t operator()(const Coordinate& key) const noexcept{
        size_t h = 0;
        h += key.x * 3 + key.y * 2 + std::pow(key.x, key.y);
        return h;
    }

    /*bool operator()(const Coordinate& coord1, const Coordinate& coord2) const {
        return (coord1.x == coord2.x && coord1.y == coord2.y);
    }*/
};