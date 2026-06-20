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
	Coordinate startingCoordinate = Coordinate(currentCoordinate.x - 5, currentCoordinate.y - 5);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
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

std::ostream& operator<<(std::ostream& out, Player& player)
{
	Coordinate startingCoordinate = Coordinate(player.currentCoordinate.x - 5, player.currentCoordinate.y - 5);
	for (int i = 0; i <= 10; ++i) {
		for (int j = 0; j <= 10; ++j) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + j, startingCoordinate.y + i);
			char temp = player.currentView[viewedCoordinate];
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
	out << std::endl;
	

	return out;
}
