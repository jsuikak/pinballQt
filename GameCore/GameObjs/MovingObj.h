#pragma once
#include "GameObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

// 运动的物体, 具有位置和速度
// 没有指定具体的形状和大小
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

	// 翻转速度的x方向
	void reverse_x_vel();
	// 翻转速度的y方向
	void reverse_y_vel();

	// 单纯地根据位置和速度计算下一时刻的位置和速度
	void update_one_frame();
	void update_one_frame(float delta_t);

	auto pos_x() const { return pos.x; }
	auto pos_y() const { return pos.y; }

	bool has_acceleration() const { return !(a == Vec2D{}); }

	// 当前时刻的位置
	Position pos;

	// 上一时刻的位置
	Position pre_pos;

	Velocity vel;

	// acceleration
	Vec2D a;
private:
	// 一帧表示多少秒
	const float delta_t_ = 1;
};

