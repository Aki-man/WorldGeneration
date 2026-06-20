#pragma once
#include "Player.h"
#include "ParallelMap.h"

class ParallelPlayer : public Player{
	ParallelWorldMap* parallelWorld;
	tbb::concurrent_hash_map<Coordinate, char, MyHashCompare> parallelView;
public:
	ParallelPlayer(Coordinate start, WorldMap& map,ParallelWorldMap* world, int view) : Player(start, map, view),
		parallelWorld(world), parallelView(tbb::concurrent_hash_map<Coordinate, char, MyHashCompare>()) {};

	
	virtual void getViewWithShadows() override;
	void getFirstQuarter(double increasePerTile, Coordinate startingCoordinate);
	void getSecondQuarter(double increasePerTile, Coordinate startingCoordinate);
	void getThirdQuearter(double increasePerTile, Coordinate startingCoordinate);
	void getFourthQuarter(double increasePerTile, Coordinate startingCoordinate);
	virtual bool checkAddingCurrentCoordinate(Coordinate coord, bool ranIntoBlock) override;
	virtual void cleanUpView() override;
	virtual bool isAdjacent(Coordinate coord, char tile) override;

	friend std::ostream& operator<<(std::ostream& out, ParallelPlayer& player);
};