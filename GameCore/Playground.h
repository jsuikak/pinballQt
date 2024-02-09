#pragma once
#include <vector>
#include <memory>
#include <qcolor.h>
#include "Common/rc.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"
struct GameOpt {
	int width = 800;
	int height = 600;
	bool balll_init_visible = true;
	QColor ball_color{ 255,255,0 };

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

	/* ��Ϸ״̬�ӿ� */
	RC game_status();

public:
	GameOpt opt;

private:
	// ������а��ӱ߽�
	void board_check_bound();
	// ����������Ƿ񴥼��߽�
	void ball_check_bound();
	// ���سߴ�
	int width_;
	int height_;

	float board_speed_ = 0.3;
	float ball_speed = 0.3;

	// �������ͼ�߽�ľ���
	int board_margin_ = 10;

	// ����
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;

	// ��Ȩ(˭����)
	BallPossession possession_ = LEFT;
};

