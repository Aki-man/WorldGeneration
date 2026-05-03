#include "coordinate.h"

Coordinate::~Coordinate()
{
	this->x = 0;
	this->y = 0;
}

bool Coordinate::operator<(const Coordinate coord) const
{
	if (this->y < coord.y) {
		return true;
	}
	else if (this->y == coord.y) {
		if (this->x < coord.x) {
			return true;
		}
		return false;
	}
	else {
		return false;
	}
}

