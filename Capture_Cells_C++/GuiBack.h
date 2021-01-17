#pragma once
#include <string>
#include <vector>
#include "Utils.h"

class GuiBack {
	std::vector<int> world;
	std::vector<std::vector<int>> worldMatrix;
	int playerOfTheTurn = 1;

public:
	std::vector<Player*> players;
	bool gameOver = false;

	GuiBack(int gridSize);
	~GuiBack();
	Player* newPlayer(std::string name, Position position, std::string color);
	int whichPlayersTurn();
	void nextTurn();
	std::vector<int> getWorld();
	std::vector<std::vector<int>> getWorldMatrix();
	void printWorld();
	int getWidth();
	int getRefIndex(int x, int y);
	int getCell(int x, int y);
	void setCell(int x, int y, Player* player);
	bool recalculateScores();
	MoveResult captureCell(int x, int y, Player* player);
};