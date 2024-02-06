#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindowClass())
{
    ui->setupUi(this);
}

mainWindow::~mainWindow()
{
    delete ui;
}
