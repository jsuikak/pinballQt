#pragma once
#include <cmath>
class Vec2D
{
public:
	Vec2D() = default;
	explicit Vec2D(float x, float y) :x(x), y(y) {}

	// 返回向量长度
	float length() { return std::sqrt(x * x + y * y); }

	// 分量乘上倍数
	void times(float x_times, float y_times);

	// 设置分量
	void set(float x, float y) { this->x = x; this->y = y; }
	Vec2D operator+(const Vec2D& rhs) const { return Vec2D{ x + rhs.x, y + rhs.y }; }
	Vec2D operator+=(const Vec2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2D operator*(const float t) { return Vec2D{ x * t, y * t }; }

	float x = 0;
	float y = 0;
};

