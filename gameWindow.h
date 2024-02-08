#pragma once

#include <QWidget>
#include <qtimer.h>

#include "ui_gameWindow.h"
#include "GameCore/Playground.h"

QT_BEGIN_NAMESPACE
namespace Ui { class gameWindowClass; };
QT_END_NAMESPACE

class gameWindow : public QWidget
{
	Q_OBJECT
public:
	gameWindow(QWidget* parent = nullptr);
	~gameWindow();

	void init_game();

	// 循环
	void game_loop();

protected:
	void paintEvent(QPaintEvent* event);

private:
	Ui::gameWindowClass* ui;
	Playground* playground_;

	// 用于绘制画面
	//std::vector<std::shared_ptr<GameObj>> objs_;
	bool running = false;
	QTimer timer_;
};
