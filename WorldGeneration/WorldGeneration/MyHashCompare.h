#pragma once
#include "coordinate.h"
#include <cmath>

struct MyHashCompare {
	static size_t hash(const Coordinate& coord) {
		size_t h = 0;
		h += coord.x * 100000 + coord.y;
		return h;
	}
	static bool equal(const Coordinate& coord1, const Coordinate& coord2) {
		return (coord1.x == coord2.x && coord1.y == coord2.y);
	}
};
