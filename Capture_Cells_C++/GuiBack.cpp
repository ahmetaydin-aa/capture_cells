#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "GuiBack.h"

GuiBack::GuiBack(int gridSize) {
	this->world.resize(gridSize * gridSize, 0);
	this->worldMatrix.resize(gridSize);
	for (int i = 0; i < gridSize; i++) {
		this->worldMatrix[i].resize(gridSize, 0);
	}
}

GuiBack::~GuiBack() {
	for (Player* player : this->players)
		delete player;
	this->players.clear();
}

Player* GuiBack::newPlayer(std::string name, Position position, std::string color) {
	Player* player = new Player(name, position, color);
	this->players.push_back(player);
	return player;
}

int GuiBack::whichPlayersTurn() {
	return this->playerOfTheTurn;
}

void GuiBack::nextTurn() {
	this->playerOfTheTurn %= 2;
	this->playerOfTheTurn += 1;
}

std::vector<int> GuiBack::getWorld() {
	return this->world;
}

std::vector<std::vector<int>> GuiBack::getWorldMatrix() {
	return this->worldMatrix;
}

void GuiBack::printWorld() {
	int width = this->getWidth();
	
	for (int i = 0; i < this->world.size(); i++) {
		int cell = this->world[i];
		if (i % width == 0)
			std::cout << "\n";
		std::cout << cell << " ";
	}
}

int GuiBack::getWidth() {
	int worldSize = this->world.size();
	int width = (int) std::sqrt(worldSize);
	return width;
}

int GuiBack::getRefIndex(int x, int y) {
	return y * this->getWidth() + x;
}

int GuiBack::getCell(int x, int y) {
	return this->world[this->getRefIndex(x, y)];
}

void GuiBack::setCell(int x, int y, Player* player) {
	this->world[this->getRefIndex(x, y)] = player->getID();
	this->worldMatrix[y][x] = player->getID();
}

bool GuiBack::recalculateScores() {
	std::vector<int> scores = Utils::calculateScores(this->world);
	this->players[0]->score = scores[1];
	this->players[1]->score = scores[2];

	if (scores[0] == 0)
		return true;
	return false;
}

MoveResult GuiBack::captureCell(int x, int y, Player* player) {
	MoveResult result = Utils::canBeCaptured(this->world, x, y, player, this->whichPlayersTurn());

	if (result.result) {
		this->setCell(x, y, player);

		for (std::vector<int> neighbor : result.neighbors) {
			this->setCell(neighbor[0], neighbor[1], player);
		}
		this->nextTurn();

		if (this->recalculateScores()) {
			this->gameOver = true;
			result.reason = "Game Over";
			return result;
		}
		player->firstTurn = false;
	}

	std::string tmpReason = result.reason;
	std::transform(tmpReason.begin(), tmpReason.end(), tmpReason.begin(), ::tolower);
	if (tmpReason.find("game over") != std::string::npos) {
		this->gameOver = true;
	}
	return result;
}
