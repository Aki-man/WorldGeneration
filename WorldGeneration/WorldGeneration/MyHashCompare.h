#pragma once
#include "coordinate.h"
#include <cmath>

template<>
struct std::hash<Coordinate>
{
	std::size_t operator()(const Coordinate& key) const noexcept
	{
		size_t h = 0;
		h += key.x * 3 + key.y * 2 + std::pow(key.x, key.y);
		return h;
	}
};

struct MyHashCompare {
	static size_t hash(const Coordinate& coord) {
		size_t h = 0;
		h += coord.x * 3 + coord.y * 2 + std::pow(coord.x, coord.y);
		return h;
	}
	static bool equal(const Coordinate& coord1, const Coordinate& coord2) {
		return (coord1.x == coord2.x && coord1.y == coord2.y);
	}
};
