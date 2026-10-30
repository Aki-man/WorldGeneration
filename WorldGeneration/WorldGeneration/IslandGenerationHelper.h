#pragma once
#include <vector>
#include "Square.h"
class IslandGenerationHelper {
	public:
		static void SetUpGeneratorAndGenerate(std::vector<char>* map, Square selectedSquare, int totalWidth);
};