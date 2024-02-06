#include "Velocity.h"

Velocity Velocity::operator+=(const Velocity& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Velocity Velocity::operator+(const Velocity& other)
{
	return Velocity(this->x + other.x, this->y + other.y);
}

void Velocity::reverse_x()
{
	x = -x;
}

void Velocity::reverse_y()
{
	y = -y;
}
