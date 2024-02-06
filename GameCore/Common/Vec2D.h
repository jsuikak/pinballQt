#pragma once
#include <cmath>
class Vec2D
{
public:
	Vec2D() = default;
	explicit Vec2D(float x, float y) :x(x), y(y) {}

	// ������������
	float length() { return std::sqrt(x * x + y * y); }

	// �������ϱ���
	void times(float x_times, float y_times);

	// ���÷���
	void set(float x, float y) { this->x = x; this->y = y; }
	float x;
	float y;
};

