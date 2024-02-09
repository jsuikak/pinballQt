#pragma once
#include "MovingObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

class ObjBall :
	public MovingObj
{
public:
	ObjBall(const Position& pos = {}, const Velocity& vel = {}, bool visible = true);
	int radius = 10; // Ð¡Çò°ë¾¶
};

