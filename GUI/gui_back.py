from .player import Position, Player
import math
from collections import Counter


class GUI(object):

    world: list = None
    players: list = []
    player_of_the_turn: int = 1

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
        if player.player_id == self.which_players_turn():
            if x < self.get_width() and y < self.get_width():
                grid_value: int = self.get_cell(x, y)
                reason: str = ""

                border_condition: bool = False
                north_cell_exists: bool = False
                east_cell_exists: bool = False
                south_cell_exists: bool = False
                west_cell_exists: bool = False
                if y - 1 >= 0:
                    border_condition = border_condition or (self.get_cell(x, y-1) == player.player_id)
                    north_cell_exists = True

                if x + 1 < self.get_width():
                    border_condition = border_condition or (self.get_cell(x + 1, y) == player.player_id)
                    east_cell_exists = True

                if y + 1 < self.get_width():
                    border_condition = border_condition or (self.get_cell(x, y + 1) == player.player_id)
                    south_cell_exists = True

                if x - 1 >= 0:
                    border_condition = border_condition or (self.get_cell(x - 1, y) == player.player_id)
                    west_cell_exists = True

                if not border_condition:
                    reason = "Selected cell doesn't exists on the player's border."

                if player.first_turn:
                    if player.position == Position.N and y == 0:
                        border_condition = True
                    elif player.position == Position.S and y == self.get_width() - 1:
                        border_condition = True
                    elif player.position == Position.W and x == 0:
                        border_condition = True
                    elif player.position == Position.E and x == self.get_width() - 1:
                        border_condition = True
                    player.first_turn = False

                    if not border_condition:
                        reason = "Please select one of the cells on your side on the first turn."

                if grid_value != 0:
                    reason = "Cell is already captured."

                if grid_value == 0 and border_condition:
                    self.set_cell(x, y, player.player_id)

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
                    return True, ""
                else:
                    return False, reason
            else:
                reason = "Selected cell is out of world boundary."
                return False, reason
        else:
            reason = "It's not your turn."
            return False, reason