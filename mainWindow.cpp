#include <thread>
#include <qtimer.h>
#include <qevent.h>
#include <qmetaobject.h>

#include "mainWindow.h"
#include "gameWindow.h"
#include "GameThread.h"
#include "Common/GameOpt.h"
#include "Common/BallPossession.h"

mainWindow::mainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainWindowClass())
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(QStringLiteral(":/resource/resource/icons/ƹ����.png")));

	GameOpt opt;
	opt.ball_speed = 10;

	this->resize(QSize(opt.width, opt.height + ui->menuBar->height()));
	game_win_ = new GameWindow(opt);
	ui->stackedWidget->addWidget(game_win_);


	/* �����ʼ��Ϸ��ť */
	connect(ui->startBtn, &QPushButton::clicked, this, [this]() {
		// ��ʼ����Ϸ
		game_win_->init_game();
		emit game_win_->playground_->a_new_round();
		ui->stackedWidget->setCurrentIndex(1);
		// ��Ϸ�¼�ѭ��
		// �¿�һ���߳� ������Qt�¼�ѭ��
		//std::thread{ std::bind(&gameWindow::game_loop, game_win_) }.detach();
		game_thread_ = new GameThread(game_win_);
		game_thread_->start();
		}
	);

	connect(game_win_, &GameWindow::backToTitle, this, [this]() {
		// ���߳�ֹͣ����
		game_thread_->stopThread();
		//game_thread_->exit(0);
		game_thread_->wait();
		//game_win_->init_game();
		delete game_thread_;
		game_thread_ = nullptr;

		// �л�ҳ��
		ui->stackedWidget->setCurrentIndex(0);
		this->setWindowTitle(QString("PinBall"));
		});

	// �µ�һ�֣����ݷ���Ȩ�޸Ĵ��ڱ���
	connect(game_win_->playground_, &Playground::a_new_round, this, [this]() {
		if (game_win_->playground_->possession() == BallPossession::LEFT) {
			this->setWindowTitle(QString("���1����"));
		}
		else {
			this->setWindowTitle(QString("���2����"));
		}
		});

	// ��ʼ��Ϸ�󣬱���ı�
	connect(game_win_, &GameWindow::gameStart, this, [this]() {
		this->setWindowTitle(QString("GO!"));
		});

	connect(ui->new_game_action, &QAction::triggered, this, [this]() {
		game_win_->init_game();
		});
}

mainWindow::~mainWindow()
{
	delete game_win_;
	delete ui;
	if (game_thread_) {
		delete game_thread_;
	}
}

