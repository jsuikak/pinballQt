#pragma once
#include "MovingObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

class ObjBall :
	public MovingObj
{
public:
	ObjBall(float radius = 10, const Position& pos = {}, const Velocity& vel = {}, bool visible = true);
	float radius; // Ð¡Çò°ë¾¶
};

