#include "ObjBoard.h"

ObjBoard::ObjBoard(float length, float width, const Position& pos, const Velocity& vel, bool visible)
	:MovingObj(pos, vel, BOARD, visible),
	length(length),
	width(width),
	half_length(length / 2),
	half_width(width / 2)
{
}
