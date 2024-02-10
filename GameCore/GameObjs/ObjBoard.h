#pragma once
#include "MovingObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

class ObjBoard :
    public MovingObj
{
public:
    ObjBoard(float length = 100.0, float width = 10.0, const Position& pos = {}, const Velocity& vel = {}, bool visible = true);
    float length ; // 板子长度
    float width ; // 板子宽度
};

