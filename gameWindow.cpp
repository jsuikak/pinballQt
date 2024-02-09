#include "gameWindow.h"
#include <qpainter.h>
#include <qdebug.h>
#include <qshortcut.h>
#include <qevent.h>

GameWindow::GameWindow(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::gameWindowClass())
{
	ui->setupUi(this);
	GameOpt opt;
	//game_ = new Game(opt);
	playground_ = new Playground(opt);
	opt_ = opt;
	this->resize(opt_.width, opt_.height);
}

GameWindow::~GameWindow()
{
	//delete game_;
	delete ui;
}

void GameWindow::init_game()
{
	playground_->init_gameobjs();
	running = true;
}

void GameWindow::game_loop()
{
	while (true) {
		// 绘制画面
		update();

		// 游戏更新一刻 
		playground_->update_one_frame();

		// 查询游戏状态
		RC status = playground_->game_status();
		if (status != RC::SUCCESS) {
			break;
		}
	}
}

void GameWindow::paintEvent(QPaintEvent* event)
{
	//qDebug() << "painEvent";
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

	// 画板子
	auto b1 = std::dynamic_pointer_cast<ObjBoard>(objs[0]);
	float half_width = b1->width / 2.0f;
	float half_length = b1->length / 2.0f;
	painter.drawRect(QRect(
		QPoint(b1->pos.x - half_width, b1->pos.y - half_length),
		QPoint(b1->pos.x + half_width, b1->pos.y + half_length)
	));

	auto b2 = std::dynamic_pointer_cast<ObjBoard>(objs[1]);
	half_width = b2->width / 2.0f;
	half_length = b2->length / 2.0f;
	painter.drawRect(QRect(
		QPoint(b2->pos.x - half_width, b2->pos.y - half_length),
		QPoint(b2->pos.x + half_width, b2->pos.y + half_length)
	));
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_W: {
		this->playground_->board_ctrl(0, 0);
	}break;
	case Qt::Key_S: {
		this->playground_->board_ctrl(0, 1);
	}break;
	case Qt::Key_Up: {
		this->playground_->board_ctrl(1, 0);
	}break;
	case Qt::Key_Down: {
		this->playground_->board_ctrl(1, 1);
	}break;
	case Qt::Key_Space: {
		// 发球
		if (playground_->possession_ == LEFT) {
			this->playground_->shoot_ball(0);
		}
		else {
			this->playground_->shoot_ball(1);
		}
	}break;

	default:
		break;
	}
}

void GameWindow::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_W:
	case Qt::Key_S: {
		this->playground_->board_ctrl(0, -1);
	}break;
	case Qt::Key_Up:
	case Qt::Key_Down: {
		this->playground_->board_ctrl(1, -1);
	}break;

	default:
		break;
	}
}

