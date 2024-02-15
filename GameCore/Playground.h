#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <qcolor.h>

#include "Common/rc.h"
#include "Common/GameState.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"
struct GameOpt {
	// ���سߴ�
	int width = 800;
	int height = 600;
	QColor background_color{ 245, 255, 253 };

	// Board
	float board_speed = 15;
	float ball_speed = 10;

	float board_margin = 20.0; // �������ͼ�߽�ľ���
	float board_length = 100.0; // ���ӳ���
	float board_width = 10.0; // ���ӿ��
	QColor board_color{ 208, 187, 134 };

	// ball
	bool ball_init_visible = true;
	float ball_radius = 10.0f;
	QColor ball_color{ 84, 93, 165 };

	float delta_t = 1; // ʱ����

	// aim
	float aim_line_margin = 5.0f;
	float aim_line_length = 20.0f;
	QColor aim_line_color{ 0, 0, 0 };

	GameOpt() = default;
	~GameOpt() = default;
};

enum BallPossession {
	LEFT, RIGHT
};

// ��Ϸ���еĳ���
// ������Ϸ�ڸ�����
// �ṩ�ı���Ϸ״̬�Ľӿ�
class Playground
{
	friend class GameWindow;
public:
	Playground(GameOpt opt);

	// ��ʼ����Ϸ����
	void init_gameobjs();

	/* ��Ϸ���ƽӿ� */
	// ���ӿ��ƣ����ú�����������һ���ٶ�
	// ָ�����Ƶ���ҡ�����(dir= 0/dir=1)
	void board_ctrl(int player, int dir);

	// С����
	// ָ����ҡ�����ĽǶ�
	void shoot_ball(int player, float angle = 45);

	/* ����ӿ� */
	// ��Ϸ����һ֡
	RC update_one_frame();

	/* ������Ϣ�ӿ� */
	// �������弯��: ����1������2����
	std::vector<std::shared_ptr<GameObj>> get_game_objs();

	const std::shared_ptr<ObjBoard>& left_board() const { return boards_[0]; }
	const std::shared_ptr<ObjBoard>& right_board() const { return boards_[1]; }

	/* ��Ϸ״̬�ӿ� */
	GameState game_state()const { return game_state_; }

public:
	GameOpt opt;

private:
	// ������а��ӱ߽�
	void board_check_bound();
	// ����������Ƿ񴥼��߽�
	void ball_check_bound();

	// ����
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;

	// ��Ȩ(˭����)
	BallPossession possession_ = LEFT;
	GameState game_state_ = UNINITIALIZED;
};

