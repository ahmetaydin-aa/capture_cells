from GUI.player import Player, Position
from GUI.gui_back import GUI


class Node(object):
    parent = None
    world: list = None
    state_score: int = None
    player: Player = None
    children: list = None
    alpha: int = -10000
    beta: int = 10000

    def __init__(self, parent, player, world):
        self.parent = parent
        self.maximizing = True if player.player_id == 1 else False
        self.player = player
        self.world = world
        self.state_score = 0
        self.children = []



class Agent(object):
    gui: GUI = None
    game_tree: list = None

    def __init__(self, player: Player, gui: GUI):
        self.player = player
        self.gui = gui

    def make_move(self):
