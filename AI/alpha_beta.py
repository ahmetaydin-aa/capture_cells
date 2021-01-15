from collections import Counter
from GUI.utils import *
import random
import copy

class AlphaBetaPruner(object):
    ALPHA = -10000
    BETA = 10000

    def __init__(self, gui):
        self.gui = gui

    def get_my_best_move(self, player_index: int = 0):
        return self.__alpha_beta_pruner(self.gui.world, 0, 15, player_index, player_index, True, self.ALPHA, self.BETA)

    def __alpha_beta_pruner(self, world, depth, max_depth, player_index, maximizing_player_index, maximizing, alpha, beta):
        player = self.gui.players[player_index]
        possible_moves = get_probable_moves(world, player)
        if player.first_turn:
            return 0, random.choice(possible_moves)
            
        if depth == max_depth or len(possible_moves) == 0:
            scores = {
                0: 0,
                1: 0,
                2: 0
            }
            for cell in world:
                scores[cell[0]] += 1
            return (scores[maximizing_player_index] - scores[(maximizing_player_index % 2) + 1]), None
        best_move = None

        while len(possible_moves) != 0:
            move = random.choice(possible_moves)
            possible_moves.remove(move)
            tmp_world = copy.deepcopy(world)
            width = get_width(tmp_world)
            x, y = get_coord_of_cell(move, width)
            tmp_world[move][0] = player.player_id
            tmp_world[move][1] = True

            for nX, nY in get_all_neighbors(x, y, width):
                if not tmp_world[get_ref_index(width, nX, nY)][1]:
                    tmp_world[get_ref_index(width, nX, nY)][0] = player.player_id

            if maximizing:
                val, _ = self.__alpha_beta_pruner(tmp_world, depth + 1, max_depth, (player_index + 1) % 2, maximizing_player_index, False, alpha, beta)
                if val > alpha:
                    best_move = move
                    alpha = val
            else:
                val, _ = self.__alpha_beta_pruner(tmp_world, depth + 1, max_depth, (player_index + 1) % 2, maximizing_player_index, True, alpha, beta)
                if val < beta:
                    best_move = move
                    beta = val
            if beta <= alpha:
                break
        return val, best_move