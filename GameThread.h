#pragma once
#include <qthread.h>
#include "GameCore/Playground.h"
class GameWindow;
class GameThread :
	public QThread
{
	Q_OBJECT
public:
	explicit GameThread(GameWindow* game_window, QObject* parent = nullptr);
protected:
	void run();
private:
	Playground* playground_{ nullptr };
	GameWindow* game_win_{ nullptr };
	bool thread_stop = false;
signals:
	void need_update();
};

