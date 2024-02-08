#pragma once
#include <vector>
#include <memory>
#include <qcolor.h>
#include "Common/rc.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"
struct GameOpt {
	int width = 60;
	int height = 40;
	bool balll_init_visible = true;
	QColor ball_color{ 255,255,0 };

	GameOpt() = default;
	~GameOpt() = default;
};
// ��Ϸ���еĳ���
// ������Ϸ�ڸ�����
// �ṩ�ı���Ϸ״̬�Ľӿ�
class Playground
{
public:
	Playground(GameOpt opt);

	// ��ʼ����Ϸ����
	void init_gameobjs();

	// ���³�������������(һ֡)
	void update_all();

	/* ��Ϸ���ƽӿ� */
	// ���ӿ��ƣ����ú�����������һ���ٶ�
	// ָ�����Ƶ���ҡ�����(dir= 0/dir=1)
	RC board_ctrl(int player, int dir);

	// С����
	// ָ����ҡ�����ĽǶ�
	RC shoot_ball(int player, float angle = 45);

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
	// ���سߴ�
	int width_;
	int height_;

	// �������ͼ�߽�ľ���
	int board_margin_ = 2;

	// ����
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;
};

