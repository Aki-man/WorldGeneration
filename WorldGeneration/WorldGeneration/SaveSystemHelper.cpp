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

std::unordered_map<std::string, int> SaveSystemHelper::GetSaveMap()
{
	std::unordered_map<std::string, int> saveDirectories;
	int counter = 1;
	for (const auto& entry : std::filesystem::directory_iterator("data/saves")) {
		saveDirectories.insert({ entry.path().filename().string(), counter});
		++counter;
	}
	return saveDirectories;
}

int SaveSystemHelper::GetSaveSize(std::string saveName)
{
	int n = 0;
	std::string fileName = "data/saves/" + saveName + "/save0.txt";
	if (std::filesystem::exists(fileName)) {
		std::ifstream file(fileName, std::ios::out | std::ios::binary);
		file >> n;
		file.close();
	}
	return n;
}



