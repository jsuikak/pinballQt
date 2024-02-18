#pragma once
#include <qcolor.h>
struct GameOpt {
	// 场地尺寸
	int width = 800;
	int height = 600;
	QColor background_color{ 245, 255, 253 };

	// Board
	float board_speed = 15;
	float ball_speed = 10;

	float board_margin = 20.0; // 板子与地图边界的距离
	float board_length = 100.0; // 板子长度
	float board_width = 10.0; // 板子宽度
	QColor board_color{ 208, 187, 134 };

	// ball
	bool ball_init_visible = false;
	float ball_radius = 10.1f;
	QColor ball_color{ 84, 93, 165 };

	float delta_t = 1; // 时间间隔

	// aim
	float aim_line_margin = 5.0f;
	float aim_line_length = 20.0f;
	QColor aim_line_color{ 0, 0, 0 };

	// control
	//Qt::Key apply_a = Qt::Key_Z;

	GameOpt() = default;
	~GameOpt() = default;
};

