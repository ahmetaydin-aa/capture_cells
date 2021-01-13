from enum import Enum


class Position(Enum):
    N = 0
    E = 1
    S = 2
    W = 3


class Player(object):
    next_id: int = 0

    def __init__(self, name: str = "Player", position: Position = Position.S) -> None:
        self.player = name
        self.player_id = self.__set_id()
        self.position = position
        self.first_turn = True
        self.score = 0

    @classmethod
    def __set_id(cls) -> int:
        cls.next_id += 1
        return cls.next_id