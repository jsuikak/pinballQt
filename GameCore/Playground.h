#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <qcolor.h>
#include <qobject.h>
#include <cmath>

#include "Common/rc.h"
#include "Common/GameOpt.h"
#include "Common/GameState.h"
#include "Common/BallPossession.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"


// ��Ϸ���еĳ���
// ������Ϸ�ڸ�����
// �ṩ�ı���Ϸ״̬�Ľӿ�
class Playground :public QObject
{
	Q_OBJECT
		friend class GameWindow;
public:
	Playground(GameOpt opt);

	// ��ʼ����Ϸ����
	void init_gameobjs();

	void init_gamestate();

	/* ��Ϸ���ƽӿ� */
	// ���ӿ��ƣ����ú�����������һ���ٶ�
	// ָ�����Ƶ���ҡ�����(dir= 0/dir=1)
	void board_ctrl(int player, int dir);

	// С����
	// ָ����ҡ�����ĽǶ�
	void shoot_ball(int player, float angle = M_PI_4);

	// ��ָ������ʩ�Ӽ��ٶ�
	void add_acceleration(MovingObj* obj, const Vec2D& a);

	void apply_acceleration(MovingObj* obj, const Vec2D& a);

	/* ����ӿ� */
	// ��Ϸ����һ֡
	void update_one_frame();

	/* ������Ϣ�ӿ� */
	// �������弯��: ����1������2����
	std::vector<std::shared_ptr<GameObj>> get_game_objs();

	std::shared_ptr<ObjBoard>& left_board() { return boards_[0]; }
	std::shared_ptr<ObjBoard>& right_board() { return boards_[1]; }
	std::shared_ptr<ObjBall>& ball(size_t index) { return balls_[index]; }

	/* ��Ϸ״̬�ӿ� */
	GameState game_state()const { return game_state_; }
	BallPossession possession() const { return possession_; }
	bool wait_to_start() const { return game_state_ == GameState::WAIT_TO_START; }
public:
	GameOpt opt;

private:
	// ������а��ӱ߽�
	void board_check_bound();
	// ����������Ƿ񴥼��߽�
	void ball_check_bound();

	// ��һ�ʤ0�� 1��
	void player_win(int player);

	// ����
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;

	// ��Ȩ(˭����)
	BallPossession possession_ = LEFT;
	GameState game_state_ = UNINITIALIZED;

	int score_p1 = 0;
	int score_p2 = 0;
signals:
	// �����»غ�
	void a_new_round();
};

