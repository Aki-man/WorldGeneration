#pragma once
#include "Coordinate.h"
#include <map>
#include "Map.h"
class Player {
public:
	
	Coordinate currentCoordinate;
	std::map<Coordinate, char> currentView;
	WorldMap& world;
	int viewSize;

	Player(Coordinate start, WorldMap& world, int view): currentCoordinate(start), currentView(std::map<Coordinate, char>()), world(world), viewSize(view) {};
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void getView();

	friend std::ostream& operator<<(std::ostream& out, Player& player);
	

};