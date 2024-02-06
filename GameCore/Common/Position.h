#pragma once
#include "Vec2D.h"
class Position :
	public Vec2D
{
public:
	Position() = default;
	explicit Position(float x, float y) :Vec2D(x, y) {}
	Position up_pos(float n);
	Position down_pos(float n);
	Position left_pos(float n);
	Position right_pos(float n);
};

