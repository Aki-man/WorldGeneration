#pragma once
#include <string>
#include <unordered_map>

struct TileColorMap {
	std::unordered_map<char, std::string> tileColorMap;
	TileColorMap() : tileColorMap(std::unordered_map<char, std::string>()) {
		this->tileColorMap['O'] = "\033[32m";
		this->tileColorMap['T'] = "\033[38;5;22m";
		this->tileColorMap['~'] = "\033[34m";
		this->tileColorMap['L'] = "\033[34m";
		this->tileColorMap['R'] = "\033[34m";
		this->tileColorMap['C'] = "\033[33m";
		this->tileColorMap['*'] = "\033[31m";
		
	}
	std::string get(char tile);
};