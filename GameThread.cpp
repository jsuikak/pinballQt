#include "GameThread.h"
#include "GameCore/Playground.h"
#include "gameWindow.h"
#include "Common/GameState.h"

GameThread::GameThread(GameWindow* game_window, QObject* parent) :QThread(parent)
{
	playground_ = game_window->playground_;
	game_win_ = game_window;
	//connect(this, &GameThread::need_update, game_window, qOverload<>(&QWidget::update));
	connect(game_win_, &GameWindow::windowClosed, this, [this]() { thread_stop = true; });
}


void GameThread::run()
{
	while (true) {
		// 绘制画面
		QThread::currentThread()->msleep(12);
		//emit need_update();
		game_win_->update();

		// 游戏更新一刻 
		playground_->update_one_frame();

		// 查询游戏状态
		GameState state = playground_->game_state();

		if (state == GameState::GAME_QIUT || thread_stop) {
			break;
		}
	}
}
