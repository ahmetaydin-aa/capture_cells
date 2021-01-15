import math
from .player import Position, Player


def get_width(world: list) -> int:
    return int(math.sqrt(len(world)))


def get_ref_index(width: int, x: int, y: int) -> int:
    return y * width + x


def get_cell(world: list, x: int, y: int) -> list:
    return world[get_ref_index(get_width(world), x, y)]


def get_coord_of_cell(ref_index: int, width: int) -> (int, int):
    return (ref_index % width), int(ref_index / width)


def get_all_neighbors(x: int, y: int, width: int) -> list:
    neighbors = []
    if y - 1 >= 0:
        neighbors.append([x, y - 1])
    if x + 1 < width:
        neighbors.append([x + 1, y])
    if y + 1 < width:
        neighbors.append([x, y + 1])
    if x - 1 >= 0:
        neighbors.append([x - 1, y])
    return neighbors


def get_probable_moves(world: list, player: Player) -> list:
    width = get_width(world)
    if player.first_turn:
        if player.position == Position.N:
            return [i for i in [get_ref_index(width, x, 0) for x in range(width)]]
        elif player.position == Position.S:
            return [i for i in [get_ref_index(width, x, width - 1) for x in range(width)]]
        elif player.position == Position.E:
            return [i for i in [get_ref_index(width, width - 1, y) for y in range(width)]]
        elif player.position == Position.W:
            return [i for i in [get_ref_index(width, 0, y) for y in range(width)]]

    current_cells = [i for i, x in enumerate(world) if ((x[0] == player.player_id) and (not x[1]))]

    def has_unoccupied_cell(index: int):
        x, y = get_coord_of_cell(index, width)
        for nX, nY in get_all_neighbors(x, y, width):
            neighbor_cell = get_cell(world, nX, nY)
            if (neighbor_cell[0] != player.player_id) and (not neighbor_cell[1]):
                return True
        return False
    return [i for i in current_cells if has_unoccupied_cell(i)]


def can_be_captured(world: list, x: int, y: int, player: Player, player_of_the_turn: int, ai_try: bool = False) -> (bool, str, list):
    if player.player_id == player_of_the_turn or ai_try:
        width = get_width(world)
        if x < width and y < width:
            grid_value: list = get_cell(world, x, y)
            ref_index: int = get_ref_index(width, x, y)
            reason: str = ""

            border_condition = True
            probable_moves = get_probable_moves(world, player)

            if ref_index not in probable_moves:
                if len(probable_moves) == 0:
                    reason = "Game Over. There is no probable move for you."
                else:
                    reason = "Selected cell doesn't exists on your border."
                border_condition = False

            if grid_value[1]:
                reason = "Cell is already captured."

            if border_condition:
                player.first_turn = False
                return True, "", get_all_neighbors(x, y, width)
            else:
                return False, reason, []
        else:
            reason = "Selected cell is out of world boundary."
            return False, reason, []
    else:
        reason = "It's not your turn."
        return False, reason, []