#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <qcolor.h>

#include "Common/rc.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"
struct GameOpt {
	// ���سߴ�
	int width = 800;
	int height = 600;
	QColor background_color{ 245, 255, 253 };

	float board_speed = 15;
	float ball_speed = 10;


	float board_margin = 20.0; // �������ͼ�߽�ľ���
	float board_length = 100.0; // ���ӳ���
	float board_width = 10.0; // ���ӿ��
	QColor board_color{ 208, 187, 134 };

	bool ball_init_visible = true;
	float ball_radius = 10.0f;
	QColor ball_color{ 84, 93, 165 };

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

	// ����
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;

	// ��Ȩ(˭����)
	BallPossession possession_ = LEFT;
};

