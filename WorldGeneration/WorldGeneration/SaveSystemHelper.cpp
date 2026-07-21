#include "SaveSystemHelper.h"

void SaveSystemHelper::CheckSaveDirectory()
{
	if (!std::filesystem::is_directory("data"))
		std::filesystem::create_directory("data");
	if (!std::filesystem::is_directory("data/saves"))
		std::filesystem::create_directory("data/saves");
}

std::vector<std::string> SaveSystemHelper::GetSaves()
{
	std::vector<std::string> saveDirectories;
	for (const auto& entry : std::filesystem::directory_iterator("data/saves")) {
		saveDirectories.push_back(entry.path().filename().string());
	}
	return saveDirectories;
}
