#include "StartGameDialog.h"
#include "ui_StartGameDialog.h"

StartGameDialog::StartGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartGameDialog)
{
    ui->setupUi(this);
    ui->treeDepthLineEdit->setText("5");
    ui->gridSizeLineEdit->setText("10");
}

int StartGameDialog::GetTreeDepth()
{
    bool ok = false;
    int depth = ui->treeDepthLineEdit->text().toInt(&ok);
    if(ok)
    {
        return depth;
    }
    else
    {
        return 5;
    }
}

int StartGameDialog::GetGridSize()
{
    bool ok = false;
    int gridSize = ui->gridSizeLineEdit->text().toInt(&ok);
    if(ok)
    {
        return gridSize;
    }
    else
    {
        return 10;
    }
}

StartGameDialog::~StartGameDialog()
{
    delete ui;
}
