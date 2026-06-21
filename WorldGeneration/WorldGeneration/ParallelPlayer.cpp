#include "ParallelPlayer.h"

void ParallelPlayer::getViewWithShadows()
{
	this->parallelView.clear();
	Coordinate startingCoordinate = Coordinate(currentCoordinate.x, currentCoordinate.y);

	//double currentIncrease = 0;
	double increasePerTile = 0;
	for (int numberOfRays = 0; numberOfRays < 20; ++numberOfRays) {
		tbb::task_group g;
		g.run([&] {this->getFirstQuarter(increasePerTile, startingCoordinate); });
		g.run([&] {this->getSecondQuarter(increasePerTile, startingCoordinate); });
		g.run([&] {this->getThirdQuarter(increasePerTile, startingCoordinate); });
		g.run([&] {this->getFourthQuarter(increasePerTile, startingCoordinate); });
		g.wait();
		/*this->getSecondQuarter(increasePerTile, startingCoordinate);
		this->getThirdQuearter(increasePerTile, startingCoordinate);
		this->getFourthQuarter(increasePerTile, startingCoordinate);*/
		increasePerTile += 0.1;
	}
	this->parallelView.insert({ startingCoordinate, '*' });
	
	//this->currentView[this->currentCoordinate] = '*';
}

void ParallelPlayer::getFirstQuarter(double increasePerTile, Coordinate startingCoordinate)
{
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
}

void ParallelPlayer::getSecondQuarter(double increasePerTile, Coordinate startingCoordinate)
{
	double currentIncrease = increasePerTile;
	bool ranIntoBlock = false;
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
}

void ParallelPlayer::getThirdQuarter(double increasePerTile, Coordinate startingCoordinate)
{
	bool ranIntoBlock = false;
	double currentIncrease = increasePerTile;
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
}

void ParallelPlayer::getFourthQuarter(double increasePerTile, Coordinate startingCoordinate)
{
	bool ranIntoBlock = false;
	double currentIncrease = increasePerTile;
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
}

bool ParallelPlayer::checkAddingCurrentCoordinate(Coordinate viewedCoordinate, bool ranIntoBlock)
{
	bool found = (*this->parallelWorld).parallelWorldMap.contains(viewedCoordinate);
	if (ranIntoBlock) {
		bool found = parallelView.contains(viewedCoordinate);
		if (!found)
			parallelView.insert({ viewedCoordinate, '+' });
	}
	else if (found) {
		char temp = (*this->parallelWorld).parallelWorldMap.at(viewedCoordinate);
		parallelView.insert({ viewedCoordinate, temp });
		if (temp == 'M' || temp == 'T')
			ranIntoBlock = true;
	}
	else {
		parallelView.insert({ viewedCoordinate, '/' });
	}
	return ranIntoBlock;
}

void ParallelPlayer::cleanUpView()
{
	Coordinate startingCoordinate = Coordinate(this->currentCoordinate.x - this->viewSize, this->currentCoordinate.y - this->viewSize);
	for (int i = 0; i <= this->viewSize * 2; ++i) {
		for (int j = 0; j <= this->viewSize * 2; ++j) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + j, startingCoordinate.y + i);
			char temp;
			
			bool found = this->parallelView.contains(viewedCoordinate);
			if (!found) {
				if (this->isAdjacent(viewedCoordinate, '+')) {
					this->parallelView.insert({ viewedCoordinate, '+'});
				
				}
				else {
					bool found = (*this->parallelWorld).parallelWorldMap.contains(viewedCoordinate);


					if (found) {
						char temp = (*this->parallelWorld).parallelWorldMap.at(viewedCoordinate);
						this->parallelView.insert({ viewedCoordinate, temp });
					}
					else {
						this->parallelView.insert({ viewedCoordinate, '/'});
					}
				}
			}
		}

	}
}

bool ParallelPlayer::isAdjacent(Coordinate coord, char tile)
{
	int adjacentCount = 0;
	Coordinate leftAdjacentTile = Coordinate(coord.x - 1, coord.y);
	Coordinate rightAdjacentTile = Coordinate(coord.x + 1, coord.y);
	Coordinate upAdjacentTile = Coordinate(coord.x, coord.y + 1);
	Coordinate downAdjacentTile = Coordinate(coord.x, coord.y - 1);
	std::vector<Coordinate> coordinatesToCheck = { leftAdjacentTile, rightAdjacentTile, upAdjacentTile, downAdjacentTile };
	for (Coordinate coord : coordinatesToCheck)
	{

		bool found = this->parallelView.contains(coord);
		if (found) {
			char foundTile = this->parallelView.at(coord);
			if (foundTile == tile) {
				adjacentCount++;
			}
		}
	}
	if (adjacentCount >= 2)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& out, ParallelPlayer& player)
{
	Coordinate startingCoordinate = Coordinate(player.currentCoordinate.x - player.viewSize, player.currentCoordinate.y - player.viewSize);
	for (int i = 0; i <= player.viewSize * 2; ++i) {
		for (int j = 0; j <= player.viewSize * 2; ++j) {
			Coordinate viewedCoordinate = Coordinate(startingCoordinate.x + j, startingCoordinate.y + i);
			char temp;
			bool found = player.parallelView.contains(viewedCoordinate);
			if (!found) {
				temp = '=';
			}
			else
				temp = player.parallelView.at(viewedCoordinate);
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
