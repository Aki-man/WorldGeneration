#pragma once
#include "Coordinate.h"
struct Square {
	Coordinate begin;
	Coordinate end;
	Square(): begin(Coordinate()), end(Coordinate()){}
	Square(int Xstart, int Xend, int Ystart, int Yend) : begin(Coordinate(Xstart, Ystart)),
		end(Coordinate(Xend, Yend)) {
	};
	bool operator==(Square square);
};