#pragma once
#include "Coordinate.h"
#include <map>
#include "Map.h"
class Player {
public:
	
	Coordinate currentCoordinate;
	std::map<Coordinate, char> currentView;
	WorldMap& world;

	Player(Coordinate start, WorldMap& world): currentCoordinate(start), currentView(std::map<Coordinate, char>()), world(world) {};
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void getView();

	friend std::ostream& operator<<(std::ostream& out, Player& player);
	

};