#pragma once
#include <filesystem>

struct SaveSystemHelper {
	static void CheckSaveDirectory();
	static std::vector<std::string> GetSaves();
};