#include <thread>
#include <qtimer.h>
#include <qevent.h>
#include <qmetaobject.h>

#include "mainWindow.h"

#include "GameThread.h"
#include "Common/GameOpt.h"
#include "Common/BallPossession.h"

mainWindow::mainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainWindowClass())
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(QStringLiteral(":/resource/resource/icons/乒乓球.png")));

	GameOpt opt;
	opt.ball_speed = 10;

	this->resize(QSize(opt.width, opt.height + ui->menuBar->height()));
	game_win_ = new GameWindow(opt);
	ui->stackedWidget->addWidget(game_win_);

	help_win_ = new HelpWindow();
	ui->stackedWidget->addWidget(help_win_);


	/* 点击开始游戏按钮 */
	connect(ui->startBtn, &QPushButton::clicked, this, [this]() {
		// 初始化游戏
		game_win_->init_game();
		emit game_win_->playground_->a_new_round();
		ui->stackedWidget->setCurrentIndex(1);
		// 游戏事件循环
		// 新开一个线程 不阻塞Qt事件循环
		//std::thread{ std::bind(&gameWindow::game_loop, game_win_) }.detach();
		game_thread_ = new GameThread(game_win_);
		game_thread_->start();
		}
	);

	/* 点击帮助按钮 */
	connect(ui->helpBtn, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(2);
		}
	);


	// 新的一局，根据发球权修改窗口标题
	connect(game_win_->playground_, &Playground::a_new_round, this, [this]() {
		if (game_win_->playground_->possession() == BallPossession::LEFT) {
			this->setWindowTitle(QString("玩家1发球"));
		}
		else {
			this->setWindowTitle(QString("玩家2发球"));
		}
		}
	);

	// 开始游戏后，标题改变
	connect(game_win_, &GameWindow::gameStart, this, [this]() {
		this->setWindowTitle(QString("GO!"));
		}
	);

	/* 菜单栏 */
	// 新游戏
	connect(ui->new_game_action, &QAction::triggered, this, [this]() {
		game_win_->init_game();
		if (ui->stackedWidget->currentIndex() != 1) {
			emit ui->startBtn->clicked();
		}
		}
	);
	// 回到标题
	connect(ui->titleAction, &QAction::triggered, this, [this]() {
		// 摧毁子线程
		if (game_thread_ != nullptr) {
			game_thread_->stopThread();
			//game_thread_->exit(0);
			game_thread_->wait();
			//game_win_->init_game();
			delete game_thread_;
			game_thread_ = nullptr;
		}

		// 切换页面
		ui->stackedWidget->setCurrentIndex(0);
		this->setWindowTitle(QString("PinBall"));
		}
	);
}

mainWindow::~mainWindow()
{
	delete game_win_;
	delete ui;
	if (game_thread_) {
		delete game_thread_;
	}
}

