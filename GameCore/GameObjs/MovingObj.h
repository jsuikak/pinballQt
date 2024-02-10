#pragma once
#include "GameObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

// �˶�������, ����λ�ú��ٶ�
// û��ָ���������״�ʹ�С
class MovingObj :
	public GameObj
{
public:
	MovingObj() = default;
	MovingObj(const Position& pos, const Velocity& vel, ObjType type = NONE, bool visible = true)
		: GameObj(type, visible), pos(pos), vel(vel)
	{
	}
	void set_pos(const Position& pos) { this->pos = pos; }
	void set_vel(const Velocity& vel) { this->vel = vel; }

	// ��ת�ٶȵ�x����
	void reverse_x_vel();
	// ��ת�ٶȵ�y����
	void reverse_y_vel();

	// �����ظ���λ�ú��ٶȼ�����һʱ�̵�λ�ú��ٶ�
	void update_one_frame();

	// ��ǰʱ�̵�λ��
	Position pos;

	// ��һʱ�̵�λ��
	Position pre_pos{ -1,-1 };

	Velocity vel;
private:
	// һ֡��ʾ������
	const float delta_t_ = 1;
};

