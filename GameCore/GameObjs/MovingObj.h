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
		: GameObj(type, visible), pos(pos), vel(vel)
	{
	}
	void set_pos(const Position& pos) { this->pos = pos; }
	void set_vel(const Velocity& vel) { this->vel = vel; }

	// 翻转速度的x方向
	void reverse_x_vel();
	// 翻转速度的y方向
	void reverse_y_vel();

	// 单纯地根据位置和速度计算下一时刻的位置和速度
	void update_one_frame();

	// 当前时刻的位置
	Position pos;

	// 上一时刻的位置
	Position pre_pos{ -1,-1 };

	Velocity vel;
private:
	// 一帧表示多少秒
	const float delta_t_ = 0.005;
};

