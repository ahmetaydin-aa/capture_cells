from collections import Counter
import random


class AlphaBetaPruner(object):
    ALPHA = -10000
    BETA = 10000

    def __init__(self, gui):
        self.gui = gui

    def get_possible_moves(self, world, player_id):
        while True:
            x = random.randint(0, self.gui.get_width())
            y = random.randint(0, self.gui.get_width())
            result, _ = self.gui.can_be_captured(x, y, self.gui.players[player_id], True)

            if result:
                north_cell_exists: bool = True if y - 1 >= 0 else False
                east_cell_exists: bool = True if x + 1 < self.gui.get_width() else False
                south_cell_exists: bool = True if y + 1 < self.gui.get_width() else False
                west_cell_exists: bool = True if x - 1 >= 0 else False

                ref_index = self.gui.get_ref_index(x, y)

                if north_cell_exists:
                    self.set_cell(x, y - 1, player.player_id)
                if east_cell_exists:
                    self.set_cell(x + 1, y, player.player_id)
                if south_cell_exists:
                    self.set_cell(x, y + 1, player.player_id)
                if west_cell_exists:
                    self.set_cell(x - 1, y, player.player_id)

                self.next_turn()
                if self.recalculate_scores():
                    return True, "Game Over"


    def alpha_beta_pruner(self, world, depth, max_depth, maximizingPlayer, alpha, beta):
        # Terminating condition. i.e
        # leaf node is reached
        if depth == max_depth:
            scores = Counter(world)
            return scores[2] - scores[1]

        if maximizingPlayer:

            best = self.ALPHA

            # Recur for left and right children
            for i in range(0, 2):

                val = minimax(depth + 1, nodeIndex * 2 + i,
                              False, values, alpha, beta)
                best = max(best, val)
                alpha = max(alpha, best)

                # Alpha Beta Pruning
                if beta <= alpha:
                    break

            return best

        else:
            best = self.BETA

            # Recur for left and
            # right children
            for i in range(0, 2):

                val = minimax(depth + 1, nodeIndex * 2 + i,
                              True, values, alpha, beta)
                best = min(best, val)
                beta = min(beta, best)

                # Alpha Beta Pruning
                if beta <= alpha:
                    break

            return best