#pragma once
#include "MovingObj.h"
#include "Common/Position.h"
#include "Common//Velocity.h"

class ObjBoard :
    public MovingObj
{
public:
    ObjBoard(const Position& pos = {}, const Velocity& vel = {}, bool visible = true);
    int length = 100; // ���ӳ���
    int width = 10; // ���ӿ��
};

