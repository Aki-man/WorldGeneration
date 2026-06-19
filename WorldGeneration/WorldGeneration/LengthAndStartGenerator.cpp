#include "LengthAndStartGenerator.h"
#include <random>

std::tuple<int, int> LengthAndStartGenerator::generateIslandLengthAndStart(int islandLength, int islandStart, int currentLength, bool getWider)
{
	std::uniform_int_distribution<int> widthChangeGenerator(1, config.randomIslandWidthChange);
	std::uniform_int_distribution<int> offsetChangeGenerator(1, config.randomIslandOffsetChange);
	int widthChange = widthChangeGenerator(rd);
	int offsetChange = offsetChangeGenerator(rd);
	if (std::rand() % 2 == 0 || (getWider && std::rand() % 3 != 0)) {
		islandLength += widthChange;
		if (islandLength > endWidth - startWidth - config.mapSeaBorderSize)
			islandLength = endWidth - startWidth - config.mapSeaBorderSize;
	}
	else {
		islandLength -= widthChange;
		if (islandLength < 0)
			islandLength = 1;
	}
	if (std::rand() % 2 == 0) {
		islandStart += offsetChange;

		if (islandStart + islandLength > startWidth + (endWidth - startWidth) - (config.mapSeaBorderSize / 2))
			islandStart = startWidth + (endWidth - startWidth) - islandLength - (config.mapSeaBorderSize / 2);
	}
	else {
		islandStart -= offsetChange;
		if (islandStart < startWidth + (config.mapSeaBorderSize / 2))
			islandStart = startWidth + (config.mapSeaBorderSize / 2);
	}
	std::tuple<int, int> returnValue = std::make_tuple(islandLength, islandStart);
	return returnValue;
}

std::tuple<int, int> LengthAndStartGenerator::generateMountainLengthAndStart(std::tuple<int, int> islandLengthAndStart, int mountainLength, int mountainStart)
{
	int islandLength = std::get<0>(islandLengthAndStart);
	int islandStart = std::get<1>(islandLengthAndStart);
	int islandEnd = islandStart + islandLength;
	std::uniform_int_distribution<int> widthChangeGenerator(0, config.randomMountainWidthChange);
	std::uniform_int_distribution<int> offsetChangeGenerator(0, config.randomMountainOffsetChange);
	int widthChange = widthChangeGenerator(rd);
	int offsetChange = offsetChangeGenerator(rd);
	if (std::rand() % 2 == 0) {
		mountainLength += widthChange;
		if (mountainLength > (islandLength / 2))
			mountainLength = islandLength / 2;
	}
	else {
		mountainLength -= widthChange;
		if (mountainLength < 0)
			mountainLength = 0;
	}
	if (std::rand() % 2 == 0) {
		mountainStart += offsetChange;

		if (mountainStart + mountainLength > islandStart + islandLength)
			mountainStart = islandStart + islandLength - mountainLength;
	}
	else {
		mountainStart -= offsetChange;
		if (mountainStart < islandStart)
			mountainStart = islandStart;
	}
	std::tuple<int, int> returnValue = std::make_tuple(mountainLength, mountainStart);
	return returnValue;
}
