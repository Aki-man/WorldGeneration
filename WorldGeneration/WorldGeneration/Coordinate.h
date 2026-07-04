#pragma once

struct Coordinate {
	int x;
	int y;
	Coordinate() : x(0), y(0) {};
	Coordinate(int x, int y) : x(x), y(y) {};
	~Coordinate();
	bool operator==(const Coordinate& coord) const;
	bool operator<(const Coordinate coord) const;
};
