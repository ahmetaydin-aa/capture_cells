from .player import Position, Player
from .utils import *
import math
from collections import Counter


class GUI(object):

    world: list = []
    players: list = []
    player_of_the_turn: int = 1
    game_over: bool = False

    def __init__(self, grid_size: int) -> None:
        for i in range(grid_size * grid_size):
            self.world.append([0, False])

    def new_player(self, name: str, position: Position, color) -> Player:
        self.players.append(Player(name, position, color))
        return self.players[-1]

    def which_players_turn(self) -> int:
        return self.player_of_the_turn

    def next_turn(self) -> None:
        self.player_of_the_turn %= 2
        self.player_of_the_turn += 1

    def get_world(self) -> list:
        return self.world

    def get_world_matrix(self) -> list:
        width = self.get_width()
        world_matrix = [map(str, self.world[i:i + width]) for i in range(0, width * width, width)]
        return world_matrix

    def print_world(self) -> None:
        world_matrix = self.get_world_matrix()
        for row in world_matrix:
            print(" ".join(row[0]))

    def get_width(self) -> int:
        return int(math.sqrt(len(self.world)))

    def get_ref_index(self, x: int, y: int) -> int:
        return y * self.get_width() + x

    def get_cell(self, x: int, y: int) -> list:
        return self.world[self.get_ref_index(x, y)]

    def set_cell(self, x: int, y: int, value: int, castle: bool) -> None:
        ref_index = self.get_ref_index(x, y)
        self.world[ref_index][0] = value
        self.world[ref_index][1] = castle

    def recalculate_scores(self) -> bool:
        scores = {
            0: 0,
            1: 0,
            2: 0
        }
        for cell in self.world:
            scores[cell[0]] += 1
        self.players[0].score = scores[1]
        self.players[1].score = scores[2]
        if scores[0] == 0:
            return True
        return False

    def capture_cell(self, x: int, y: int, player: Player) -> (bool, str, list):
        result, reason, neighbors = can_be_captured(self.world, x, y, player, self.which_players_turn())
        captured_neighbors = []

        if result:
            self.set_cell(x, y, player.player_id, True)

            for nX, nY in get_all_neighbors(x, y, self.get_width()):
                if not self.get_cell(nX, nY)[1]:
                    self.set_cell(nX, nY, player.player_id, False)
                    captured_neighbors.append([nX, nY])

            self.next_turn()
            if self.recalculate_scores():
                self.game_over = True
                return True, "Game Over", captured_neighbors
        if "game over" in reason.lower():
            self.game_over = True
        return result, reason, captured_neighbors