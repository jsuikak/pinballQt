#include <thread>
#include <qtimer.h>
#include <qmetaobject.h>

#include "mainWindow.h"
#include "gameWindow.h"
#include "GameThread.h"

mainWindow::mainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainWindowClass())
{
	ui->setupUi(this);
	game_win_ = new GameWindow();
	connect(ui->startBtn, &QPushButton::clicked, this, [this]() {
		/* ��ʼ��Ϸ */
		// ��ʼ����Ϸ
		game_win_->init_game();
		// ��ʾ����
		game_win_->show();
		// ��Ϸ�¼�ѭ��
		// �¿�һ���߳� ������Qt�¼�ѭ��
		//std::thread{ std::bind(&gameWindow::game_loop, game_win_) }.detach();
		(new GameThread(game_win_, this))->start();

		//QTimer::singleShot(0, this, [this]() {
		//	game_win_->game_loop();
		//	});

		}
	);
}

mainWindow::~mainWindow()
{
	delete game_win_;
	delete ui;
}
