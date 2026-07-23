#pragma once
#include <unordered_map>
#include <filesystem>

struct SaveSystemHelper {
	static void CheckSaveDirectory();
	static std::vector<std::string> GetSaves();
	static std::unordered_map<std::string, int> GetSaveMap();
};