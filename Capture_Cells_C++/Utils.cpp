#include <iostream>
#include <cmath>
#include <algorithm>
#include "Utils.h"

int Utils::getWidth(std::vector<int> world) {
	int worldSize = world.size();
    int width = (int)std::sqrt(worldSize);
	return width;
}

int Utils::getRefIndex(int width, int x, int y) {
	return y * width + x;
}

int Utils::getCell(std::vector<int> world, int x, int y) {
	return world[Utils::getRefIndex(Utils::getWidth(world), x, y)];
}

std::vector<int> Utils::calculateScores(std::vector<int> world) {
	std::vector<int> scores;
	scores.resize(3, 0);
	for (int cell : world) {
		scores[cell] += 1;
	}
	return scores;
}

std::vector<int> Utils::getCoordOfCell(int refIndex, int width) {
	std::vector<int> result;
	result.resize(2, 0);
	result[0] = (refIndex % width);
	result[1] = (int)(refIndex / width);
	return result;
}

std::vector<std::vector<int>> Utils::getAllNeighbors(int x, int y, int width) {
	std::vector<std::vector<int>> neighbors;
	if (y - 1 >= 0) {
		std::vector<int> neighbor;
		neighbor.push_back(x);
		neighbor.push_back(y - 1);

		neighbors.push_back(neighbor);
	}
	if (x + 1 < width) {
		std::vector<int> neighbor;
		neighbor.push_back(x + 1);
		neighbor.push_back(y);

		neighbors.push_back(neighbor);
	}
	if (y + 1 < width) {
		std::vector<int> neighbor;
		neighbor.push_back(x);
		neighbor.push_back(y + 1);

		neighbors.push_back(neighbor);
	}
	if (x - 1 >= 0) {
		std::vector<int> neighbor;
		neighbor.push_back(x - 1);
		neighbor.push_back(y);

		neighbors.push_back(neighbor);
	}
	return neighbors;
}

bool Utils::hasOccupiedCell(std::vector<int> world, int width, int index, Player* player) {
	std::vector<int> coords = Utils::getCoordOfCell(index, width);

	for (std::vector<int> neighbor : Utils::getAllNeighbors(coords[0], coords[1], width)) {
		if (Utils::getCell(world, neighbor[0], neighbor[1]) == player->getID())
			return true;
	}
	return false;
}

std::vector<int> Utils::getProbableMoves(std::vector<int> world, Player* player) {
	int width = Utils::getWidth(world); 
	std::vector<int> probableMoves;

	if (player->firstTurn) {
		for (int i = 0; i < width; i++)
			switch (player->position)
			{
			case Position::N:
				probableMoves.push_back(Utils::getRefIndex(width, i, 0));
				break;
			case Position::S:
				probableMoves.push_back(Utils::getRefIndex(width, i, width - 1));
				break;
			case Position::E:
				probableMoves.push_back(Utils::getRefIndex(width, width - 1, i));
				break;
			case Position::W:
				probableMoves.push_back(Utils::getRefIndex(width, 0, i));
				break;
			default:
				break;
			}
	}
	else {
		for (unsigned int i = 0; i < world.size(); i++) {
			int cell = world[i];
			if (cell == 0) {
				if (Utils::hasOccupiedCell(world, width, i, player)) {
					probableMoves.push_back(i);
				}
			}
		}
	}

	return probableMoves;
}

MoveResult Utils::canBeCaptured(std::vector<int> world, int x, int y, Player* player, int playerOfTurn, bool aiTry) {
	MoveResult result;
	if ((player->getID() == playerOfTurn) || aiTry) {
		int width = Utils::getWidth(world);

		if ((x < width) && (y < width)) {
			int cell = Utils::getCell(world, x, y);
			int refIndex = Utils::getRefIndex(width, x, y);
			std::string reason = "";

			bool borderCondition = true;
			std::vector<int> probableMoves = Utils::getProbableMoves(world, player);

			if (!(std::find(probableMoves.begin(), probableMoves.end(), refIndex) != probableMoves.end())) {
				if (probableMoves.size() == 0)
					reason = "Game Over. There is no probable move for you.";
				else
					reason = "Selected cell doesn't exists on your border.";
				borderCondition = false;
			}

			if (cell != 0)
				reason = "Cell is already captured.";

			if (cell == 0 && borderCondition) {
				result.result = true;
				result.reason = reason;
				result.neighbors = Utils::getAllNeighbors(x, y, width);
			}
			else {
				result.result = false;
				result.reason = reason;
			}
		}
		else {
			result.result = false;
			result.reason = "Selected cell is out of world boundary.";
		}
	}
	else {
		result.result = false;
		result.reason = "It's not your turn.";
	}

	return result;
}
