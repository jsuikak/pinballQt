#pragma once
#include "MovingObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

class ObjBoard :
    public MovingObj
{
public:
    ObjBoard(const Position& pos = {}, const Velocity& vel = {}, bool visible = true);
    int length = 40; // 板子长度
    int width = 10; // 板子宽度
};

