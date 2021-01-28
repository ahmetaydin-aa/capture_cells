#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>

#include <iostream>
#include <vector>
#include <time.h>
#include <windows.h>
#include <Lmcons.h>
#include <fstream>
#include "GuiBack.h"
#include "GuiFront.h"
#include "Utils.h"
#include "Player.h"
#include "AlphaBetaPruner.h"
#include "StartGameDialog.h"
#include <QObject>

const std::string LOG_FILE_NAME = "log_file.dat";
bool isLogFileExists();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    GetUserNameA(username, &username_len);
    GuiFront* front = nullptr;

    StartGameDialog strtGame;
    int result = strtGame.exec();
    if(result == 1)
    {
        int gridSize = strtGame.GetGridSize();
        int treeDepth = strtGame.GetTreeDepth();
        GuiBack* backend = new GuiBack(gridSize);
        front = new GuiFront(backend, gridSize, treeDepth);
        if (!isLogFileExists()) {
            front->log_file.open(LOG_FILE_NAME, std::fstream::app | std::fstream::binary);
            std::string user = username;
            Utils::writeHashedDataToLog(&(front->log_file), user + "\n");
        } else {
            front->log_file.open(LOG_FILE_NAME, std::fstream::app | std::fstream::binary);
        }

        front->show();
    }
    return a.exec();
}

bool isLogFileExists() {
    if (FILE *file = fopen(LOG_FILE_NAME.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

