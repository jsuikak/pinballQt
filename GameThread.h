#pragma once
#include <qthread.h>
#include "GameCore/Playground.h"
class GameWindow;
// 职责：驱动PlayGround内的物体运动，通知Gamewindow进行画面绘制
class GameThread :
	public QThread
{
	Q_OBJECT
public:
	explicit GameThread(GameWindow* game_window, QObject* parent = nullptr);
	~GameThread();
	void stopThread() { thread_stop = true; }
	bool running() const { return !thread_stop; }
protected:
	void run();
private:
	Playground* playground_{ nullptr };
	GameWindow* game_win_{ nullptr };
	bool thread_stop = false;
	//signals:
	//	void need_update();
};

