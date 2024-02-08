#pragma once
#include "MovingObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

class ObjBoard :
    public MovingObj
{
public:
    ObjBoard(const Position& pos = {}, const Velocity& vel = {}, bool visible = true);
    int length = 10; // °å×Ó³¤¶È
};

