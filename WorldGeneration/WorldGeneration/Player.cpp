#include "Player.h"

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

void Player::getViewWithShadows() {
	this->currentView.clear();
	Coordinate startingCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y);
	
	//double currentIncrease = 0;
	double increasePerTile = 0;
	for (int numberOfRays = -this->viewSize; numberOfRays < this->viewSize; ++numberOfRays) {
		
		bool ranIntoBlock = false;
		double currentIncrease = 0;
		for (int i = 1; i <= this->viewSize * 2; ++i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y + int(currentIncrease));
			if (ranIntoBlock) {
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
			currentIncrease += increasePerTile;
			//currentIncrease += increasePerTile/2;
		}
		ranIntoBlock = false;
		currentIncrease = 0;
		for (int i = 1; i <= this->viewSize * 2; ++i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y - int(currentIncrease));
			if (ranIntoBlock) {
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
			currentIncrease += increasePerTile;
			//currentIncrease += increasePerTile/2;
		}
		currentIncrease = 0;
		ranIntoBlock = false;
		for (int i = -1; i >= -this->viewSize * 2; --i) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y - int(currentIncrease));
			if (ranIntoBlock) {
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
			currentIncrease += increasePerTile;
			//currentIncrease += increasePerTile / 2;
		}
		ranIntoBlock = false;
		currentIncrease = 0;
		for (int i = -1; i >= -this->viewSize * 2; --i) {

			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + i, startingCoordinate.y + int(currentIncrease));
			if (ranIntoBlock) {
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
			currentIncrease += increasePerTile;
			//currentIncrease += increasePerTile/2;
		}
		increasePerTile += 0.1;
	}
	this->currentView[this->currentCoordinate] = '*';
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
