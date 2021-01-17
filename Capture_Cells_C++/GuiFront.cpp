#include "GuiFront.h"
#include <iostream>
#include <vector>
#include <QtWidgets/QPushButton>

GuiFront::GuiFront(GuiBack* backend, int gridSize) : QWidget() {
    this->setWindowTitle("Capture Cells");
    this->backend = backend;
    this->aiInstance = backend->newPlayer("AI", Position::S, "yellow");
    this->pInstance = backend->newPlayer("Player", Position::N, "blue");

    this->gridSize = gridSize;
    this->pruner = new AlphaBetaPruner(backend);
    this->layout = new QGridLayout();
    this->layout->setSpacing(0);

    for (int i = 0; i < gridSize*gridSize; i++) {
        CellButton* button = new CellButton(i);
        button->setStyleSheet("background-color: white;");
        int buttonSize = (int) (this->SCREEN_RESOLUTION / gridSize);
        button->setFixedSize(buttonSize, buttonSize);
        connect(button, &CellButton::clicked, this, &GuiFront::on_click);
        this->buttons.push_back(button);

        std::vector<int> coords = Utils::getCoordOfCell(i, gridSize);
        this->layout->addWidget(button, coords[1], coords[0]);
    }
    this->setLayout(this->layout);

    std::srand(time(NULL));
    MoveResult result;
    std::vector<int> firstMoves;
    std::vector<int> coords;
    int randMoveIndex = -1;

    firstMoves = Utils::getProbableMoves(backend->getWorld(), aiInstance);
    randMoveIndex = std::rand() % firstMoves.size();
    coords = Utils::getCoordOfCell(firstMoves[randMoveIndex], backend->getWidth());
    std::cout << coords[0] << " - " << coords[1] << " -- " << "\n";
    result = backend->captureCell(coords[0], coords[1], aiInstance);
    result.neighbors.push_back(coords);
    this->paintCells(this->aiInstance, result.neighbors);
}

GuiFront::~GuiFront() {
    for (CellButton* button : this->buttons) {
        delete button;
    }
    this->buttons.clear();

    delete this->aiInstance;
    delete this->pInstance;
    delete this->backend;
    delete this->layout;
    delete this->pruner;
}

void GuiFront::paintCells(Player *player, std::vector<std::vector<int>> cells) {
    std::string color = player->getColor();
    std::string bgTxt = "background-color: " + color + ";";
    for (std::vector<int> cell : cells) {
        int refIndex = Utils::getRefIndex(this->gridSize, cell[0], cell[1]);
        this->buttons[refIndex]->setStyleSheet(bgTxt.c_str());
    }
}

void GuiFront::on_click() {
    CellButton* sender = (CellButton*) this->sender();
    int refIndex = sender->getId();
    std::vector<int> coords;
    std::vector<int> move;
    MoveResult result;

    coords = Utils::getCoordOfCell(refIndex, this->gridSize);
    result = this->backend->captureCell(coords[0], coords[1], this->pInstance);

    if (result.result && !backend->gameOver) {
        result.neighbors.push_back(coords);
        this->paintCells(this->pInstance, result.neighbors);
        move = this->pruner->getMyBestMove(0);
        
        if (move[1] != -1) {
            coords = Utils::getCoordOfCell(move[1], this->gridSize);
            std::cout << coords[0] << " - " << coords[1] << " -- " << move[0] << "\n";
            result = this->backend->captureCell(coords[0], coords[1], this->aiInstance);
            result.neighbors.push_back(coords);
            this->paintCells(this->aiInstance, result.neighbors);
        }
        else {
            this->backend->gameOver = true;
        }
    }

    std::cout << "AI: " << this->aiInstance->score << "\n";
    std::cout << "Player: " << this->pInstance->score << "\n\n";
}

CellButton::CellButton(int id) {
    this->id = id;
}

int CellButton::getId() {
    return this->id;
}