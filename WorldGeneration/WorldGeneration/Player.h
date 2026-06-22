#pragma once
#include "Coordinate.h"
#include "HashInjection.h"
#include <map>
#include "Map.h"
class Player {
public:
	
	Coordinate currentCoordinate;
	std::unordered_map<Coordinate, char> currentView;
	WorldMap& world;
	int viewSize;

	Player(Coordinate start, WorldMap& world, int view): currentCoordinate(start), currentView(std::unordered_map<Coordinate, char>()), world(world), viewSize(view) {};
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void getView();
	void save(std::string saveName);
	void load(std::string saveName);
	virtual void getViewWithShadows();
	virtual bool checkAddingCurrentCoordinate(Coordinate coord, bool ranIntoBlock);
	virtual void cleanUpView();
	virtual bool isAdjacent(Coordinate coord, char tile);

	friend std::ostream& operator<<(std::ostream& out, Player& player);
	

};