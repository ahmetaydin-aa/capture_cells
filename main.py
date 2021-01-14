import sys
import enum
import concurrent.futures
from GUI import GUI, Position, Player
from GUI.utils import *
from AI.alpha_beta import AlphaBetaPruner
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QGridLayout, QPushButton
from PyQt5.QtCore import pyqtSlot

SCREEN_RESOLUTION = 500
buttons_by_coordinate = {}
cell_status = {}
coordinates_by_button = {}
player_instance = None
ai_instance = None
backend = None
grid_size = 25

def window(grid_size):
    app = QApplication(sys.argv)
    widget = QWidget()
    layout = QGridLayout()
    layout.setSpacing(0)

    @pyqtSlot()
    def on_click():
        coord = coordinates_by_button[app.sender()]
        valid_action, backend_message, cells = backend.capture_cell(coord[0], coord[1], player_instance)
        if valid_action and not backend.game_over:
            app.sender().setStyleSheet(f'background-color: {player_instance.get_color()};')    
            paint_cells(player_instance, cells)
            move_score, move = pruner.get_my_best_move(0)
            if move != None:
                x, y = get_coord_of_cell(move, backend.get_width())
                print("x:", x, "- y:", y, " --", move_score)
                _, _, cells = backend.capture_cell(x, y, ai_instance)
                cells.append([x,y])
                paint_cells(ai_instance, cells)
            else:
                backend.game_over = True

    for i in range(grid_size):
        for j in range(grid_size):
            # keep a reference to the buttons
            button = QPushButton()
            button.setStyleSheet("background-color: white;")
            button.setFixedSize(SCREEN_RESOLUTION//grid_size, SCREEN_RESOLUTION//grid_size)  # bunu
            button.clicked.connect(on_click)

            buttons_by_coordinate[(j, i)] = button
            coordinates_by_button[button] = (j, i)
            # add to the layout
            layout.addWidget(button, i, j)

    move_score, move = pruner.get_my_best_move(0)
    x, y = get_coord_of_cell(move, backend.get_width())
    print("x:", x, "- y:", y, " --", move_score)
    _, _, cells = backend.capture_cell(x, y, ai_instance)
    cells.append([x,y])
    paint_cells(ai_instance, cells)

    widget.setLayout(layout)
    widget.show()
    sys.exit(app.exec_())


def paint_cells(player, cells_to_be_painted) -> None:
    color = player.get_color()
    for cell in  cells_to_be_painted:
        buttons_by_coordinate[(cell[0], cell[1])].setStyleSheet(f'background-color: {color};')    

if __name__ == "__main__":
    backend = GUI(grid_size)
    ai_instance = backend.new_player("AI", Position.S, 'yellow')
    player_instance = backend.new_player("Player", Position.N, 'blue')
    pruner = AlphaBetaPruner(backend)
    window(grid_size)

    # while not gui.game_over:
    #     gui.print_world()
    #     print(p1.player + ":", p1.score)
    #     print(p2.player + ":", p2.score)

    #     # move_score_2, move_2 = pruner.get_my_best_move(1)
    #     # if move_2 != None:
    #     #     x, y = get_coord_of_cell(move_2, gui.get_width())
    #     #     print("x:", x, "- y:", y, " --", move_score, "(", p2.player_id, ")")
    #     #     gui.capture_cell(x, y, p2)
    #     # else:
    #     #     gui.game_over = True
    #     #     break
    #     #
    #     # move_score_1, move_1 = pruner.get_my_best_move(0)
    #     # if move_1 != None:
    #     #     x, y = get_coord_of_cell(move_1, gui.get_width())
    #     #     print("x:", x, "- y:", y, " --", move_score, "(", p1.player_id, ")")
    #     #     gui.capture_cell(x, y, p1)
    #     # else:
    #     #     gui.game_over = True
    #     #     break

    #     p2_move = list(map(int, input("Your Move: ").replace(" ", "").split(",")))
    #     result, reason = gui.capture_cell(p2_move[0], p2_move[1], p2)

    #     if result:
    #         move_score, move = pruner.get_my_best_move(0)
    #         if move != None:
    #             x, y = get_coord_of_cell(move, gui.get_width())
    #             print("x:", x, "- y:", y, " --", move_score)
    #             print(gui.capture_cell(x, y, p1))
    #         else:
    #             gui.game_over = True
    #     else:
    #         print(reason)

    # print("Game Over")
    # print(p1.player + ":", p1.score)
    # print(p2.player + ":", p2.score)
    # gui.print_world()