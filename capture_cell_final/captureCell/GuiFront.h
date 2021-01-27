#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <iostream>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "Player.h"
#include "Utils.h"
#include "GuiBack.h"
#include "AlphaBetaPruner.h"


class CellButton: public QPushButton {
    Q_OBJECT
    int id;

public:
    CellButton(int id);
    int getId();
};


class GuiFront: public QWidget {
    Q_OBJECT
    const int SCREEN_RESOLUTION = 500;
    int gridSize = 5;
    std::vector<CellButton*> buttons;
    Player* aiInstance;
    Player* pInstance;
    GuiBack* backend;
    AlphaBetaPruner* pruner;
    QGridLayout* gameLayout;
    QLabel* playerScore;
    QLabel* AIScore;

public slots:
    void on_click();

public:
    GuiFront(QWidget* parent);
    GuiFront(GuiBack* backend, int gridSize = 5, int depthSize = 12);
    ~GuiFront();
    void paintCells(Player* player, std::vector<std::vector<int>> cells);
};
