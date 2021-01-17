#pragma once
#include <vector>
#include "Player.h"

struct MoveResult {
	bool result;
	std::string reason;
	std::vector<std::vector<int>> neighbors;
};

class Utils {
	static bool hasOccupiedCell(std::vector<int> world, int width, int index, Player* player);

public:
	static int getWidth(std::vector<int> world);
	static int getRefIndex(int width, int x, int y);
	static int getCell(std::vector<int> world, int x, int y);
	static std::vector<int> calculateScores(std::vector<int> world);
	static std::vector<int> getCoordOfCell(int refIndex, int width);
	static std::vector<std::vector<int>> getAllNeighbors(int x, int y, int width);
	static std::vector<int> getProbableMoves(std::vector<int> world, Player* player);
	static MoveResult canBeCaptured(std::vector<int> world, int x, int y, Player* player, int playerOfTurn, bool aiTry=false);
};