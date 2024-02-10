#include "ObjBoard.h"

ObjBoard::ObjBoard(float length, float width, const Position& pos, const Velocity& vel, bool visible)
	:MovingObj(pos, vel, BOARD, visible),
	length(length),
	width(width)
{
}
