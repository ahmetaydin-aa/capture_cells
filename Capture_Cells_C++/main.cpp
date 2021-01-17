#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>

#include <iostream>
#include <vector>
#include <time.h>
#include "GuiBack.h"
#include "GuiFront.h"
#include "Utils.h"
#include "Player.h"
#include "AlphaBetaPruner.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int gridSize = 6;
    GuiBack* backend = new GuiBack(gridSize);
    GuiFront* front = new GuiFront(backend, gridSize);

    front->show();
    return a.exec();
}
