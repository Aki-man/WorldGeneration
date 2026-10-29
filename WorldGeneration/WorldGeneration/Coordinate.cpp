#include "coordinate.h"
#include <ostream>

Coordinate::~Coordinate()
{
	this->x = 0;
	this->y = 0;
}

bool Coordinate::operator==(const Coordinate& coord) const
{
	if (this->y == coord.y && this->x == coord.x) {
		return true;
	}
	return false;
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

std::ostream& operator<<(std::ostream& os, const Coordinate& coord)
{
	os << "x:" << coord.x << " y:" << coord.y;
	return os;
}

