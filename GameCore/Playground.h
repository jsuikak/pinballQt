#pragma once
#include <vector>
#include <memory>
#include <qcolor.h>
#include "Common/rc.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"
struct GameOpt {
	int width = 800;
	int height = 600;
	bool balll_init_visible = true;
	QColor ball_color{ 255,255,0 };

	GameOpt() = default;
	~GameOpt() = default;
};

enum BallPossession {
	LEFT, RIGHT
};

// 游戏进行的场地
// 管理游戏内各物体
// 提供改变游戏状态的接口
class Playground
{
	friend class GameWindow;
public:
	Playground(GameOpt opt);

	// 初始化游戏物体
	void init_gameobjs();

	/* 游戏控制接口 */
	// 板子控制，调用函数后会给板子一个速度
	// 指定控制的玩家、上下(dir= 0/dir=1)
	void board_ctrl(int player, int dir);

	// 小球发射
	// 指定玩家、发射的角度
	void shoot_ball(int player, float angle = 45);

	/* 计算接口 */
	// 游戏更新一帧
	RC update_one_frame();

	/* 物体信息接口 */
	// 返回物体集合: 板子1、板子2、球
	std::vector<std::shared_ptr<GameObj>> get_game_objs();

	/* 游戏状态接口 */
	RC game_status();

public:
	GameOpt opt;

private:
	// 检查所有板子边界
	void board_check_bound();
	// 检查所有球是否触及边界
	void ball_check_bound();
	// 场地尺寸
	int width_;
	int height_;

	float board_speed_ = 0.3;
	float ball_speed = 0.3;

	// 板子与地图边界的距离
	int board_margin_ = 10;

	// 物体
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;

	// 球权(谁发球)
	BallPossession possession_ = LEFT;
};

