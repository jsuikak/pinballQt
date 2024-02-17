#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <qcolor.h>
#include <qobject.h>
#include <cmath>

#include "Common/rc.h"
#include "Common/GameOpt.h"
#include "Common/GameState.h"
#include "Common/BallPossession.h"
#include "GameObjs/ObjBall.h"
#include "GameObjs/ObjBoard.h"


// 游戏进行的场地
// 管理游戏内各物体
// 提供改变游戏状态的接口
class Playground :public QObject
{
	Q_OBJECT
		friend class GameWindow;
public:
	Playground(GameOpt opt);

	// 初始化游戏物体
	void init_gameobjs();

	void init_gamestate();

	/* 游戏控制接口 */
	// 板子控制，调用函数后会给板子一个速度
	// 指定控制的玩家、上下(dir= 0/dir=1)
	void board_ctrl(int player, int dir);

	// 小球发射
	// 指定玩家、发射的角度
	void shoot_ball(int player, float angle = M_PI_4);

	// 向指定物体施加加速度
	void add_acceleration(MovingObj* obj, const Vec2D& a);

	void apply_acceleration(MovingObj* obj, const Vec2D& a);

	/* 计算接口 */
	// 游戏更新一帧
	void update_one_frame();

	/* 物体信息接口 */
	// 返回物体集合: 板子1、板子2、球
	std::vector<std::shared_ptr<GameObj>> get_game_objs();

	std::shared_ptr<ObjBoard>& left_board() { return boards_[0]; }
	std::shared_ptr<ObjBoard>& right_board() { return boards_[1]; }
	std::shared_ptr<ObjBall>& ball(size_t index) { return balls_[index]; }

	/* 游戏状态接口 */
	GameState game_state()const { return game_state_; }
	BallPossession possession() const { return possession_; }
	bool wait_to_start() const { return game_state_ == GameState::WAIT_TO_START; }
public:
	GameOpt opt;

private:
	// 检查所有板子边界
	void board_check_bound();
	// 检查所有球是否触及边界
	void ball_check_bound();

	// 玩家获胜0左 1右
	void player_win(int player);

	// 物体
	std::vector<std::shared_ptr<ObjBall>> balls_;
	std::vector<std::shared_ptr<ObjBoard>> boards_;

	// 球权(谁发球)
	BallPossession possession_ = LEFT;
	GameState game_state_ = UNINITIALIZED;

	int score_p1 = 0;
	int score_p2 = 0;
signals:
	// 开启新回合
	void a_new_round();
};

