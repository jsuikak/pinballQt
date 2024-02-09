#include "GameThread.h"
#include "GameCore/Playground.h"
#include "gameWindow.h"

GameThread::GameThread(GameWindow* game_window, QObject* parent) :QThread(parent)
{
	playground_ = game_window->playground_;
	game_win_ = game_window;
	//connect(this, &GameThread::need_update, game_window, qOverload<>(&QWidget::update));
	//connect(game_win_, &QWidget::close, this, std::bind(&QThread::exit,this,0));
}


void GameThread::run()
{
	while (true) {
		// 绘制画面
		//emit need_update();
		game_win_->update();

		// 游戏更新一刻 
		playground_->update_one_frame();

		// 查询游戏状态
		RC status = playground_->game_status();
		if (status != RC::SUCCESS) {
			break;
		}
	}
}
