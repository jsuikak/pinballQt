#include "ObjBall.h"

ObjBall::ObjBall(float radius, const Position& pos, const Velocity& vel, bool visible) :
	MovingObj(pos, vel, BALL, visible),
	radius(radius)
{
}
