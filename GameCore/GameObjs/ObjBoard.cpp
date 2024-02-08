#include "ObjBoard.h"

ObjBoard::ObjBoard(const Position& pos, const Velocity& vel, bool visible) :MovingObj(pos, vel, BOARD, visible)
{
}
