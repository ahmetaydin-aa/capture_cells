#include "GuiFront.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
GuiFront::GuiFront(GuiBack* backend, int gridSize, int depthSize) : QWidget() {
    this->setWindowTitle("Capture Cells");
    this->backend = backend;
    this->aiInstance = backend->newPlayer("AI", Position::S, "yellow");
    this->pInstance = backend->newPlayer("Player", Position::N, "blue");
    this->AIScore = new QLabel("0");
    this->playerScore = new QLabel("0");
    this->gridSize = gridSize;
    this->pruner = new AlphaBetaPruner(backend, depthSize);
    this->gameLayout = new QGridLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    this->gameLayout->setSpacing(0);
    //this->layout_beginning = new QGridLayout();

    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addWidget(new QLabel("AI Score:"));
    hBox->addWidget(this->AIScore);
    hBox->addWidget(new QLabel("Player Score:"));
    hBox->addWidget(this->playerScore);

    for (int i = 0; i < gridSize*gridSize; i++) {
        CellButton* button = new CellButton(i);
        button->setStyleSheet("background-color: white;");
        int buttonSize = (int) (this->SCREEN_RESOLUTION / gridSize);
        button->setFixedSize(buttonSize, buttonSize);
        connect(button, &CellButton::clicked, this, &GuiFront::on_click);
        this->buttons.push_back(button);

        std::vector<int> coords = Utils::getCoordOfCell(i, gridSize);
        this->gameLayout->addWidget(button, coords[1], coords[0]);
    }
    mainLayout->addLayout(gameLayout,0);
    this->setLayout(mainLayout);
    mainLayout->addLayout(hBox, 1 );

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
    delete this->gameLayout;
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
    if(backend->gameOver)
    {
        QMessageBox msg;
        QString result = aiInstance->score > pInstance->score ? "AI Won!": "You Won!";
        msg.setText(result);
        msg.setWindowTitle("Game Over");
        msg.setIcon(QMessageBox::Icon::Critical);
        msg.exec();
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
    std::cout << "AI: " << this->aiInstance->score << "\n";
    AIScore->setText(QString::number(aiInstance->score));
    playerScore->setText(QString::number(pInstance->score));
    std::cout << "Player: " << this->pInstance->score << "\n\n";
}

GuiFront::GuiFront(QWidget *parent)
    : QWidget(parent)
{

}

CellButton::CellButton(int id) {
    this->id = id;
}

int CellButton::getId() {
    return this->id;
}
