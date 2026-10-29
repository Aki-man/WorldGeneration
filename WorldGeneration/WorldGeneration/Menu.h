#pragma once
#include <iostream>
#include "Map.h"
#include "Player.h"
#include <tbb/tick_count.h>
#include "ParallelPlayer.h"
#include "Tests.h"
#include "SaveSystemHelper.h"

struct Menu {
	static std::string getSaveName();
	static void PlayerMenu(std::string name, Player& newPlayer);
	static void ParallelPlayerMenu(std::string name, ParallelPlayer& newPlayer);
	static void generateNewIslandMenu(std::string name);
	static void loadIslandMenu();
	static void testsMenu();
	static void playMenu();
	static void mainMenu();
};