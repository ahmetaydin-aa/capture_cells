from GUI import GUI, Position, Player

if __name__ == "__main__":
    gui = GUI(5)
    p1 = gui.new_player("Player", Position.S)
    p2 = gui.new_player("AI", Position.N)

    print(gui.get_world())

    print(gui.capture_cell(4, 4, p1))
    print(p1.score)
    print(p2.score)

    print(gui.capture_cell(2, 0, p2))
    print(p1.score)
    print(p2.score)

    print(gui.capture_cell(4, 2, p1))
    print(p1.score)
    print(p2.score)

    print(gui.capture_cell(3, 1, p2))
    print(p1.score)
    print(p2.score)


    print(gui.get_world())