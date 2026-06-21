#pragma once
#include "Player.h"
#include "ParallelMap.h"
#include "MyEquality.h"
#include "MyHasher.h"

class ParallelPlayer : public Player{
	ParallelWorldMap* parallelWorld;
	tbb::concurrent_unordered_map<Coordinate, char> parallelView;
public:
	ParallelPlayer(Coordinate start, WorldMap& map,ParallelWorldMap* world, int view) : Player(start, map, view),
		parallelWorld(world), parallelView(tbb::concurrent_unordered_map<Coordinate, char>()) {};

	
	virtual void getViewWithShadows() override;
	void getFirstQuarter(double increasePerTile, Coordinate startingCoordinate);
	void getSecondQuarter(double increasePerTile, Coordinate startingCoordinate);
	void getThirdQuarter(double increasePerTile, Coordinate startingCoordinate);
	void getFourthQuarter(double increasePerTile, Coordinate startingCoordinate);
	virtual bool checkAddingCurrentCoordinate(Coordinate coord, bool ranIntoBlock) override;
	virtual void cleanUpView() override;
	virtual bool isAdjacent(Coordinate coord, char tile) override;

	friend std::ostream& operator<<(std::ostream& out, ParallelPlayer& player);
};