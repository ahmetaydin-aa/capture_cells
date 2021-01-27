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
#include "StartGameDialog.h"
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartGameDialog strtGame;
    int result = strtGame.exec();
    if(result == 1)
    {
        int gridSize = strtGame.GetGridSize();
        int treeDepth = strtGame.GetTreeDepth();
        GuiBack* backend = new GuiBack(gridSize);
        GuiFront* front = new GuiFront(backend, gridSize, treeDepth);

        front->show();


    }
    return a.exec();
}
