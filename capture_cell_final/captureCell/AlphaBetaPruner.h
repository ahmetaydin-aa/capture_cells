#pragma once
#include <vector>
#include "GuiBack.h"


class AlphaBetaPruner {
	const int ALPHA = -10000;
	const int BETA = 10000;
	
	GuiBack* backend = nullptr;
    int depth;

	std::vector<int> prune(std::vector<int> world, int depth, int maxDepth, int pIndex, int maximizingPlayerIndex, bool maximizing, int alpha, int beta);
public:
    AlphaBetaPruner(GuiBack* backend, int depthSize);
	~AlphaBetaPruner();
	std::vector<int> getMyBestMove(int pIndex = 0);
};
