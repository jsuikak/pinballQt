#pragma once
#include "Vec2D.h"
class Velocity :
    public Vec2D
{
public:
    Velocity() {}
    explicit Velocity(float x, float y) :Vec2D(x, y) {}
    float speed() { return Vec2D::length(); }

    // ��ת�ٶȵ�x����
    void reverse_x();
    // ��ת�ٶȵ�y����
    void reverse_y();
private:
};

