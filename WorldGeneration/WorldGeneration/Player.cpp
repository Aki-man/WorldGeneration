#include "Player.h"
#include <fstream>
#include <vector>
#include <filesystem>

void Player::moveUp()
{
	this->currentCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y-1);
}

void Player::moveDown()
{
	this->currentCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y + 1);
}

void Player::moveLeft()
{
	this->currentCoordinate = Coordinate(currentCoordinate.x - 1, currentCoordinate.y);
}

void Player::moveRight()
{
	this->currentCoordinate = Coordinate(currentCoordinate.x + 1, currentCoordinate.y);
}

void Player::getView()
{
	Coordinate startingCoordinate = Coordinate(currentCoordinate.x - this->viewSize, currentCoordinate.y - this->viewSize);
	for (int i = 0; i <= this->viewSize*2; ++i) {
		for (int j = 0; j <= this->viewSize*2; ++j) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + j, startingCoordinate.y + i);
			if (this->world.worldMap.contains(viewedCoordinate)) {
				this->currentView[viewedCoordinate] = this->world.worldMap[viewedCoordinate];
			}
			else {
				this->currentView[viewedCoordinate] = '/';
			}
		}
	}
	this->currentView[this->currentCoordinate] = '*';
}

void Player::save(std::string saveName)
{
	std::filesystem::path path = saveName;
	if (!std::filesystem::is_directory(path))
		std::filesystem::create_directory(path);
	std::string fileName = saveName + "/savePlayer.txt";
	std::ofstream file(fileName, std::ios::out | std::ios::binary);
	file << this->currentCoordinate.x << " " << this->currentCoordinate.y;
}

void Player::load(std::string saveName)
{
	std::string fileName = saveName + "/savePlayer.txt";
	std::ifstream file(fileName, std::ios::out | std::ios::binary);
	int x;
	int y;
	file >> x >> y;
	this->currentCoordinate = Coordinate(x, y);
}

void Player::getViewWithShadows() {
	this->currentView.clear();
	Coordinate startingCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y);
	
	//double currentIncrease = 0;
	double increasePerTile = 0;
	for (int numberOfRays = 0; numberOfRays < 20; ++numberOfRays) {
		
		bool ranIntoBlock = false;
		double currentIncrease = increasePerTile;
		for (int i = 1; i <= this->viewSize * 2; ++i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y + int(currentIncrease));
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		ranIntoBlock = false;
		currentIncrease = increasePerTile;
		for (int i = 1; i <= this->viewSize * 2; ++i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y - int(currentIncrease));
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		
		}
		currentIncrease = increasePerTile;
		ranIntoBlock = false;
		for (int i = -1; i >= -this->viewSize * 2; --i) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y - int(currentIncrease));
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		ranIntoBlock = false;
		currentIncrease = increasePerTile;
		for (int i = -1; i >= -this->viewSize * 2; --i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y + int(currentIncrease));
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		ranIntoBlock = false;
		currentIncrease = increasePerTile;
		for (int i = 1; i <= this->viewSize * 2; ++i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + int(currentIncrease), startingCoordinate.y + i);
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		ranIntoBlock = false;
		currentIncrease = increasePerTile;
		for (int i = -1; i >= -this->viewSize * 2; --i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + int(currentIncrease), startingCoordinate.y + i);
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		ranIntoBlock = false;
		currentIncrease = increasePerTile;
		for (int i = 1; i <= this->viewSize * 2; ++i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x - int(currentIncrease), startingCoordinate.y + i);
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		ranIntoBlock = false;
		currentIncrease = increasePerTile;
		for (int i = -1; i >= -this->viewSize * 2; --i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x - int(currentIncrease), startingCoordinate.y + i);
			ranIntoBlock = this->checkAddingCurrentCoordinate(viewedCoordinate, ranIntoBlock);
			currentIncrease += increasePerTile;
		}
		increasePerTile += 0.1;
	}
	this->currentView[this->currentCoordinate] = '*';
}

bool Player::checkAddingCurrentCoordinate(Coordinate viewedCoordinate, bool ranIntoBlock)
{
	if (ranIntoBlock) {
		if(!this->currentView.contains(viewedCoordinate))
			this->currentView[viewedCoordinate] = '+';
	}
	else if (this->world.worldMap.contains(viewedCoordinate)) {
		char temp = this->world.worldMap[viewedCoordinate];
		this->currentView[viewedCoordinate] = temp;
		if (temp == 'M' || temp == 'T')
			ranIntoBlock = true;
	}
	else {
		this->currentView[viewedCoordinate] = '/';
	}
	return ranIntoBlock;
}

void Player::cleanUpView() {
	Coordinate startingCoordinate = Coordinate(this->currentCoordinate.x -this->viewSize,this->currentCoordinate.y - this->viewSize);
	for (int i = 0; i <= this->viewSize * 2; ++i) {
		for (int j = 0; j <= this->viewSize * 2; ++j) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + j, startingCoordinate.y + i);
			char temp;
			if (!this->currentView.contains(viewedCoordinate)) {
				if (this->isAdjacent(viewedCoordinate, '+'))
					this->currentView[viewedCoordinate] = '+';
				else
					if (this->world.worldMap.contains(viewedCoordinate))
						this->currentView[viewedCoordinate] = this->world.worldMap[viewedCoordinate];
					else
						this->currentView[viewedCoordinate] = '/';
			}
		}

	}
}

bool Player::isAdjacent(Coordinate coord, char tile) {
	int adjacentCount = 0;
	Coordinate leftAdjacentTile = Coordinate(coord.x - 1, coord.y);
	Coordinate rightAdjacentTile = Coordinate(coord.x + 1, coord.y);
	Coordinate upAdjacentTile = Coordinate(coord.x, coord.y + 1);
	Coordinate downAdjacentTile = Coordinate(coord.x, coord.y - 1);
	std::vector<Coordinate> coordinatesToCheck = { leftAdjacentTile, rightAdjacentTile, upAdjacentTile, downAdjacentTile };
	for (Coordinate coord : coordinatesToCheck)
	{
		if (this->currentView.contains(coord)) {
			char foundTile = this->currentView[coord];
			if (foundTile == tile) {
				adjacentCount++;
			}
		}
	}
	if(adjacentCount >=2)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& out, Player& player)
{
	Coordinate startingCoordinate = Coordinate(player.currentCoordinate.x - player.viewSize, player.currentCoordinate.y - player.viewSize);
	for (int i = 0; i <= player.viewSize*2; ++i) {
		for (int j = 0; j <= player.viewSize*2; ++j) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + j, startingCoordinate.y + i);
			char temp;
			if (!player.currentView.contains(viewedCoordinate)) {
				temp = '=';
			}
			else
				temp = player.currentView[viewedCoordinate];
			if (temp == 'O') {
				out << "\033[32m";
			}
			else if (temp == 'T') {
				out << "\033[38;5;22m";
			}
			else if (temp == '~' || temp == 'L' || temp == 'R') {
				out << "\033[34m";
			}
			else if (temp == 'C') {
				out << "\033[33m";
			}
			else if (temp == '*') {
				out << "\033[31m";
			}
			else {
				out << "\x1b[0m";
			}
			out << temp;
		}
		
		out << std::endl;
	}
	out << std::endl << "X:" << player.currentCoordinate.x << " Y:" << player.currentCoordinate.y;
	out << std::endl;
	

	return out;
}
