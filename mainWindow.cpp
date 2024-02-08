#include <thread>
#include "mainWindow.h"
#include "gameWindow.h"

mainWindow::mainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainWindowClass())
{
	ui->setupUi(this);
	game_win_ = new gameWindow();
	connect(ui->startBtn, &QPushButton::clicked, this, [this]() {
		// ��ʼ��Ϸ
		game_win_->init_game();
		game_win_->show();
		// �¿�һ���߳̽�����Ϸ�¼�ѭ��, ������Qt�¼�ѭ��
		std::thread t(std::bind(&gameWindow::game_loop, game_win_));
		t.detach();
		//game_win_->game_loop();
		});
}

mainWindow::~mainWindow()
{
	delete game_win_;
	delete ui;
}
