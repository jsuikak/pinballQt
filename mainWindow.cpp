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
		/* 开始游戏 */
		// 初始化游戏
		game_win_->init_game();
		// 显示窗口
		game_win_->show();
		// 游戏事件循环
		// 新开一个线程 不阻塞Qt事件循环
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
