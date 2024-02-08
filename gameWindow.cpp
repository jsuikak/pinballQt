#include "gameWindow.h"
#include <qpainter.h>
#include <qdebug.h>
#include <Windows.h>

gameWindow::gameWindow(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::gameWindowClass())
{
	ui->setupUi(this);
	GameOpt opt{};
	//game_ = new Game(opt);
	playground_ = new Playground(opt);

	//timer_.setInterval(50);
}

gameWindow::~gameWindow()
{
	//delete game_;
	delete ui;
}

void gameWindow::init_game()
{
	playground_->init_gameobjs();
	running = true;
}

void gameWindow::game_loop()
{
	// 阻塞程序? 
	while (true) {
		// 绘制画面
		update();

		// 获取游戏状态
		//RC status = playground_->game_status();
	}
}

void gameWindow::paintEvent(QPaintEvent* event)
{
	qDebug() << "painEvent";
	// 获取所有物体
	const auto& objs = playground_->get_game_objs();
	QPainter painter(this);
	painter.setPen(QColor(255, 0, 0));
	painter.setBrush(QColor(255, 255, 255));

	// 画圆
	auto ball = std::dynamic_pointer_cast<ObjBall>(objs[2]);
	painter.drawEllipse(QRect(
		QPoint(ball->pos.x, ball->pos.y),
		QSize(ball->radius * 2, ball->radius * 2)
	));
}

