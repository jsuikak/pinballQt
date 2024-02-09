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
		// ���ƻ���
		//emit need_update();
		game_win_->update();

		// ��Ϸ����һ�� 
		playground_->update_one_frame();

		// ��ѯ��Ϸ״̬
		RC status = playground_->game_status();
		if (status != RC::SUCCESS) {
			break;
		}
	}
}
