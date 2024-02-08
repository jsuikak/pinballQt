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
		// 开始游戏
		game_win_->init_game();
		game_win_->show();
		// 新开一个线程进行游戏事件循环, 不阻塞Qt事件循环
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
