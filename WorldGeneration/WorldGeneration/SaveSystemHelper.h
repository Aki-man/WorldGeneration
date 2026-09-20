#pragma once
#include <unordered_map>
#include <filesystem>
#include <fstream>

struct SaveSystemHelper {
	static void CheckSaveDirectory();
	static std::vector<std::string> GetSaves();
	static std::unordered_map<std::string, int> GetSaveMap();
	static int GetSaveSize(std::string saveName);
};