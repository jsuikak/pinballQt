#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::HelpWindowClass())
{
	ui->setupUi(this);
}

HelpWindow::~HelpWindow()
{
	delete ui;
}
