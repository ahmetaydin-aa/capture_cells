
#include <vector>
#include <algorithm>
#include "AlphaBetaPruner.h"

AlphaBetaPruner::AlphaBetaPruner(GuiBack* backend, int depthSize) {
	this->backend = backend;
    this->depth = depthSize;
}

AlphaBetaPruner::~AlphaBetaPruner() {
	//delete this->backend;
}

std::vector<int> AlphaBetaPruner::getMyBestMove(int pIndex) {
    return this->prune(backend->getWorld(), 0, depth, pIndex, pIndex, false, this->ALPHA, this->BETA);
}

std::vector<int> AlphaBetaPruner::prune(std::vector<int> world, int depth, int maxDepth, int pIndex, int maximizingPlayerIndex, bool maximizing, int alpha, int beta) {
	std::vector<int> result;
	result.resize(2, -1);

	Player* player = this->backend->players[pIndex];
	int width = Utils::getWidth(world);
	std::vector<int> possibleMoves = Utils::getProbableMoves(world, player);
    if (possibleMoves.size() > this->maxBranching) {
        this->maxBranching = possibleMoves.size();
    }

	if (depth == maxDepth || possibleMoves.size() == 0) {
		std::vector<int> scores = Utils::calculateScores(world);
		result[0] = (scores[maximizingPlayerIndex] - scores[(maximizingPlayerIndex % 2) + 1]);
	}
	else {
		int bestMove = -1;
		int best = -1000000000;

		std::sort(possibleMoves.begin(), possibleMoves.end(), std::greater<int>());

		if (maximizing)
			best = this->ALPHA;
		else
			best = this->BETA;

		for (int move : possibleMoves) {
			std::vector<int> tmpWorld(world);
			std::vector<int> coords = Utils::getCoordOfCell(move, width);
			tmpWorld[move] = player->getID();

			for (std::vector<int> neighbor : Utils::getAllNeighbors(coords[0], coords[1], width))
				tmpWorld[Utils::getRefIndex(width, neighbor[0], neighbor[1])] = player->getID();

			if (maximizing) {
				//best = this->ALPHA;
				std::vector<int> childRes = this->prune(tmpWorld, depth + 1, maxDepth, (pIndex + 1) % 2, maximizingPlayerIndex, false, alpha, beta);
				best = std::max(best, childRes[0]);
				if (best > alpha) {
					bestMove = move;
					alpha = best;
				}
			}
			else {
				//best = this->BETA;
				std::vector<int> childRes = this->prune(tmpWorld, depth + 1, maxDepth, (pIndex + 1) % 2, maximizingPlayerIndex, true, alpha, beta);
				best = std::min(best, childRes[0]);
				if (best < beta) {
					bestMove = move;
					beta = best;
				}
			}

			if (beta <= alpha)
				break;
		}

		result[0] = best;
		result[1] = bestMove;
    }

	return result;
}

unsigned long long AlphaBetaPruner::getMaxBraching() {
    return this->maxBranching;
}
