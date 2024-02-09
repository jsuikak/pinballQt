#pragma once

#include <QWidget>
#include <qtimer.h>

#include "ui_gameWindow.h"
#include "GameCore/Playground.h"

QT_BEGIN_NAMESPACE
namespace Ui { class gameWindowClass; };
QT_END_NAMESPACE

class GameWindow : public QWidget
{
	friend class GameThread;
	Q_OBJECT
public:
	GameWindow(QWidget* parent = nullptr);
	~GameWindow();

	void init_game();

	// 循环
	void game_loop();

protected:
	void paintEvent(QPaintEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	
private:

	Ui::gameWindowClass* ui;
	Playground* playground_;

	// 用于绘制画面
	//std::vector<std::shared_ptr<GameObj>> objs_;
	bool running = false;
	GameOpt opt_;
	//QTimer timer{ this };
};
