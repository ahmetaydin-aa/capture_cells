from collections import Counter
from GUI.utils import *
import random


class AlphaBetaPruner(object):
    ALPHA = -10000
    BETA = 10000

    def __init__(self, gui):
        self.gui = gui

    def get_my_best_move(self, player_index: int = 0):
        return self.__alpha_beta_pruner(self.gui.world, 0, 5, player_index, player_index, True, self.ALPHA, self.BETA)

    def __alpha_beta_pruner(self, world, depth, max_depth, player_index, maximizing_player_index, maximizing, alpha, beta):
        player = self.gui.players[player_index]
        possible_moves = get_probable_moves(world, player)
        if depth == max_depth or len(possible_moves) == 0:
            scores = Counter(world)
            return (scores[maximizing_player_index] - scores[(maximizing_player_index % 2) + 1]), None
        best_move = None

        while len(possible_moves) != 0:
            move = random.choice(possible_moves)
            possible_moves.remove(move)
            tmp_world = list(world)
            width = get_width(tmp_world)
            x, y = get_coord_of_cell(move, width)
            tmp_world[move] = player.player_id

            for nX, nY in get_all_neighbors(x, y, width):
                tmp_world[get_ref_index(width, nX, nY)] = player.player_id

            if maximizing:
                best = self.ALPHA
                val, _ = self.__alpha_beta_pruner(tmp_world, depth + 1, max_depth, (player_index + 1) % 2, player_index, False, alpha, beta)
                best = max(best, val)
                if best > alpha:
                    best_move = move
                alpha = max(alpha, best)
                if beta <= alpha:
                    break
            else:
                best = self.BETA
                val, _ = self.__alpha_beta_pruner(tmp_world, depth + 1, max_depth, (player_index + 1) % 2, player_index, True, alpha, beta)
                best = min(best, val)
                if best < beta:
                    best_move = move
                beta = min(beta, best)
                if beta <= alpha:
                    break
        return best, best_move