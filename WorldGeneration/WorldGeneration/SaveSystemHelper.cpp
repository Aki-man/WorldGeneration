#include "SaveSystemHelper.h"

void SaveSystemHelper::CheckSaveDirectory()
{
	if (!std::filesystem::is_directory("data"))
		std::filesystem::create_directory("data");
	if (!std::filesystem::is_directory("data/saves"))
		std::filesystem::create_directory("data/saves");
}
