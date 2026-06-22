#pragma once
#include <string>
class Tests {
public:
	static void ParallelGenerationTest(int n);
	static void ParallelOneIslandGenerationTest(int n);
	static void ParallelViewTest(int n);
	static void SaveWorldToFileTest(int n, std::string fileName);
	static void SaveWorldToFileTestSerial(int n, std::string fileName);
	static void BatchTests(int islandSize, int viewSize, std::string fileName);
};