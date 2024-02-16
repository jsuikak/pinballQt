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
		: GameObj(type, visible), pos(pos), pre_pos(pos), vel(vel)
	{
	}
	virtual ~MovingObj() {}
	void set_pos(const Position& pos) { this->pos = pos; }
	void set_vel(const Velocity& vel) { this->vel = vel; }

	// ��ת�ٶȵ�x����
	void reverse_x_vel();
	// ��ת�ٶȵ�y����
	void reverse_y_vel();

	// �����ظ���λ�ú��ٶȼ�����һʱ�̵�λ�ú��ٶ�
	void update_one_frame();
	void update_one_frame(float delta_t);

	auto pos_x() const { return pos.x; }
	auto pos_y() const { return pos.y; }

	bool has_acceleration() const { return !(a == Vec2D{}); }

	// ��ǰʱ�̵�λ��
	Position pos;

	// ��һʱ�̵�λ��
	Position pre_pos;

	Velocity vel;

	// acceleration
	Vec2D a;
private:
	// һ֡��ʾ������
	const float delta_t_ = 1;
};

