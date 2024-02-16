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
	opt.ball_speed = 10;
	//game_ = new Game(opt);
	playground_ = new Playground(opt);
	opt_ = opt;
	this->resize(opt_.width, opt_.height);

	connect(playground_, &Playground::a_new_round, this, [this]() {
		if (BallPossession::LEFT == playground_->possession_) {
			angle_ = M_PI_4;
		}
		else {
			angle_ = M_PI - M_PI_4;
		}
		});
	//setMouseTracking(true);
}

GameWindow::~GameWindow()
{
	//delete game_;
	delete ui;
}

void GameWindow::init_game()
{
	playground_->init_gameobjs();
	running_ = true;
}

void GameWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (aiming_) {
		QPoint p = event->pos();
		if (playground_->possession_ == LEFT) {
			const Position p_b = playground_->left_board()->pos;
			angle_ = atan2f(p.y() - p_b.y, p.x() - p_b.x);
		}
		else {
			const Position p_b = playground_->right_board()->pos;
			angle_ = atan2f(p.y() - p_b.y, p.x() - p_b.x);
		}
	}
}

void GameWindow::mousePressEvent(QMouseEvent* event)
{
	aiming_ = true;
}

void GameWindow::mouseReleaseEvent(QMouseEvent* event)
{
	aiming_ = false;
}

void GameWindow::paintEvent(QPaintEvent* event)
{
	//qDebug() << "painEvent";
	// 获取所有物体
	const auto& objs = playground_->get_game_objs();
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// 画背景
	painter.setPen(opt_.background_color);
	painter.setBrush(opt_.background_color);
	painter.drawRect(this->rect());

	// 画球
	//auto ball = std::dynamic_pointer_cast<ObjBall>(objs[2]);
	ObjBall* ball = dynamic_cast<ObjBall*>(objs[2].get());
	if (ball->visible) {
		painter.setPen(opt_.ball_color);
		painter.setBrush(opt_.ball_color);
		float radius = ball->radius;
		painter.drawEllipse(QRect(
			QPoint(ball->pos.x - radius, ball->pos.y - radius),
			QSize(ball->radius * 2, ball->radius * 2)
		));
	}

	// 画板子
	auto&& b1 = playground_->left_board();
	if (b1->visible) {
		painter.setPen(opt_.board_color);
		painter.setBrush(opt_.board_color);
		float half_width = b1->width / 2.0f;
		float half_length = b1->length / 2.0f;
		//auto b1 = std::dynamic_pointer_cast<ObjBoard>(objs[0]);
		painter.drawRect(QRect(
			QPoint(b1->pos.x - half_width, b1->pos.y - half_length),
			QPoint(b1->pos.x + half_width, b1->pos.y + half_length)
		));
	}

	auto&& b2 = playground_->right_board();
	if (b2->visible) {
		painter.setPen(opt_.board_color);
		painter.setBrush(opt_.board_color);
		float half_width = b2->width / 2.0f;
		float half_length = b2->length / 2.0f;
		painter.drawRect(QRect(
			QPoint(b2->pos.x - half_width, b2->pos.y - half_length),
			QPoint(b2->pos.x + half_width, b2->pos.y + half_length)
		));
	}

	// 瞄准线
	if (aiming_) {
		float r = opt_.aim_line_length;
		painter.setPen(opt_.aim_line_color);
		if (playground_->possession_ == LEFT) {
			Position b_p = playground_->left_board()->pos;
			QPoint p1(b_p.x + playground_->left_board()->half_width + opt_.aim_line_margin, b_p.y);
			QPoint delta(r * cosf(angle_), r * sinf(angle_));
			painter.drawLine(p1, p1 + delta);
		}
		else {
			Position b_p = playground_->right_board()->pos;
			QPoint p1(b_p.x - playground_->left_board()->half_width - opt_.aim_line_margin, b_p.y);
			QPoint delta(r * cosf(angle_), r * sinf(angle_));
			painter.drawLine(p1, p1 + delta);
		}
	}

	// 加速度
	painter.setPen(QPen(Qt::red, 3));
	for (std::shared_ptr<ObjBall>& ball : playground_->balls_) {
		if (ball->has_acceleration()) {
			Position ball_pos = ball->pos;
			const float scale = 75;
			//qDebug() << "draw line";
			painter.drawLine(
				QPoint(ball_pos.x, ball_pos.y),
				QPoint(ball_pos.x + scale * ball->a.x, ball_pos.y + scale * ball->a.y)
			);
		}
	}
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
		if (playground_->game_state() == GameState::WAIT_TO_START) {
			if (playground_->possession_ == LEFT) {
				//qDebug() << "shoot ball with angle:" << angle_;
				this->playground_->shoot_ball(0, angle_);
			}
			else {
				//qDebug() << "shoot ball with angle:" << angle_;
				this->playground_->shoot_ball(1, angle_);
			}
			playground_->game_state_ = GameState::GOING_ON;
		}
	}break;
	case Qt::Key_Z: {
		//playground_->add_acceleration(playground_->ball(0).get(), Vec2D{ 0,1 });
		playground_->apply_acceleration(playground_->ball(0).get(), Vec2D{ 0, 0.5 });
	}break;
	case Qt::Key_0: {
		playground_->apply_acceleration(playground_->ball(0).get(), Vec2D{ 0, -0.5 });
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
	case Qt::Key_Z:
	case Qt::Key_0: {
		playground_->apply_acceleration(playground_->ball(0).get(), Vec2D{ 0, 0 });
	}break;

	default:
		break;
	}
}

void GameWindow::closeEvent(QCloseEvent* event)
{
	emit windowClosed();
	// 关闭窗口
	event->accept();
}

