#include "MovingObj.h"

void MovingObj::reverse_x_vel()
{
	this->vel.reverse_x();
}

void MovingObj::reverse_y_vel()
{
	this->vel.reverse_y();
}

void MovingObj::update_one_frame()
{
	update_one_frame(delta_t_);
}

void MovingObj::update_one_frame(float delta_t)
{
	pre_pos = pos;
	pos += vel * delta_t;
}
