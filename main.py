from GUI import GUI, Position, Player
from GUI.utils import *
from AI.alpha_beta import AlphaBetaPruner

if __name__ == "__main__":
    gui = GUI(10)
    p1 = gui.new_player("AI-1", Position.S)
    p2 = gui.new_player("AI-2", Position.N)
    pruner = AlphaBetaPruner(gui)

    move_score, move = pruner.get_my_best_move(0)
    x, y = get_coord_of_cell(move, gui.get_width())
    print("x:", x, "- y:", y, " --", move_score)
    print(gui.capture_cell(x, y, p1))

    while not gui.game_over:
        gui.print_world()
        print(p1.player + ":", p1.score)
        print(p2.player + ":", p2.score)

        # move_score_2, move_2 = pruner.get_my_best_move(1)
        # if move_2 != None:
        #     x, y = get_coord_of_cell(move_2, gui.get_width())
        #     print("x:", x, "- y:", y, " --", move_score)
        #     print(gui.capture_cell(x, y, p2))
        # else:
        #     gui.game_over = True
        #     break
        #
        # move_score_1, move_1 = pruner.get_my_best_move(0)
        # if move_1 != None:
        #     x, y = get_coord_of_cell(move_1, gui.get_width())
        #     print("x:", x, "- y:", y, " --", move_score)
        #     print(gui.capture_cell(x, y, p1))
        # else:
        #     gui.game_over = True
        #     break

        p2_move = list(map(int, input("Your Move: ").replace(" ", "").split(",")))
        result, reason = gui.capture_cell(p2_move[0], p2_move[1], p2)

        if result:
            move_score, move = pruner.get_my_best_move(0)
            if move != None:
                x, y = get_coord_of_cell(move, gui.get_width())
                print("x:", x, "- y:", y, " --", move_score)
                print(gui.capture_cell(x, y, p1))
            else:
                gui.game_over = True
        else:
            print(reason)

    print("Game Over")
    print(p1.player + ":", p1.score)
    print(p2.player + ":", p2.score)
    gui.print_world()