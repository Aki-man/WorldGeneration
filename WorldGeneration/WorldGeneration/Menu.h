#pragma once
#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"
#include "Tests.h"
#include "SaveSystemHelper.h"

struct Menu {
	static void saveMapMenu(WorldMap& map, Player& player);
	static void PlayerMenu(WorldMap& map, Player& newPlayer);
	static void ParallelPlayerMenu(WorldMap& map, ParallelPlayer& newPlayer);
	static void generateNewIslandMenu();
	static void loadIslandMenu();
	static void testsMenu();
	static void mainMenu();
};