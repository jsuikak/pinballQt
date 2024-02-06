#pragma once
#include "Vec2D.h"
class Velocity :
    public Vec2D
{
public:
    Velocity() {}
    explicit Velocity(float x, float y) :Vec2D(x, y) {}
    float speed() { return Vec2D::length(); }
    Velocity operator+=(const Velocity& other);
    Velocity operator+(const Velocity& other);

    // 翻转速度的x方向
    void reverse_x();
    // 翻转速度的y方向
    void reverse_y();
private:
};

