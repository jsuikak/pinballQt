#pragma once

#include <QWidget>
#include <qtimer.h>
#include <cmath>
#include <qdebug.h>

#include "ui_gameWindow.h"
#include "GameCore/Playground.h"

QT_BEGIN_NAMESPACE
namespace Ui { class gameWindowClass; };
QT_END_NAMESPACE

class GameWindow : public QWidget
{
	friend class GameThread;
	friend class mainWindow;
	Q_OBJECT
public:
	~GameWindow();
	GameWindow(GameOpt opt, QWidget* parent = nullptr);

	void init_game();
	auto opt() -> GameOpt const { return opt_; }
	void updateOpt(GameOpt opt);
protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
private:
	Ui::gameWindowClass* ui;
	Playground* playground_;

	// ���ڻ��ƻ���
	//std::vector<std::shared_ptr<GameObj>> objs_;
	//QPainter *painter{ nullptr };

	// ��Ϸ���ڽ���
	//bool running_ = false;

	// ��׼��
	bool aiming_ = false;
	// ��׼�Ƕ�
	float angle_ = M_PI_4;

	GameOpt opt_;
	//QTimer timer{ this };

signals:
	void backToTitle();
	void gameStart();
};
