from .player import Position, Player
from .utils import *
import math
from collections import Counter


class GUI(object):

    world: list = None
    players: list = []
    player_of_the_turn: int = 1
    game_over: bool = False

    def __init__(self, grid_size: int) -> None:
        self.world = [0 for _ in range(grid_size * grid_size)]

    def new_player(self, name: str, position: Position) -> Player:
        self.players.append(Player(name, position))
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
        width = self.get_width()
        world_matrix = [map(str, self.world[i:i + width]) for i in range(0, width*width, width)]
        for row in world_matrix:
            print(" ".join(row))

    def get_width(self) -> int:
        return int(math.sqrt(len(self.world)))

    def get_ref_index(self, x: int, y: int) -> int:
        return y * self.get_width() + x

    def get_cell(self, x: int, y: int) -> int:
        return self.world[self.get_ref_index(x, y)]

    def set_cell(self, x: int, y: int, value: int) -> None:
        self.world[self.get_ref_index(x, y)] = value

    def recalculate_scores(self) -> bool:
        scores = Counter(self.world)
        self.players[0].score = scores[1]
        self.players[1].score = scores[2]
        if scores[0] == 0:
            return True
        return False

    def capture_cell(self, x: int, y: int, player: Player) -> (bool, str):
        result, reason = can_be_captured(self.world, x, y, player, self.which_players_turn())

        if result:
            self.set_cell(x, y, player.player_id)

            for nX, nY in get_all_neighbors(x, y, self.get_width()):
                self.set_cell(nX, nY, player.player_id)

            self.next_turn()
            if self.recalculate_scores():
                self.game_over = True
                return True, "Game Over"
        if "game over" in reason.lower():
            self.game_over = True
        return result, reason