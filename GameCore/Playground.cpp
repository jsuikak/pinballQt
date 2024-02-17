#include "Playground.h"
#include <qdebug.h>
#include <qscopedpointer.h>

Playground::Playground(GameOpt opt) :opt(opt)
{
	balls_.emplace_back(std::make_shared<ObjBall>(opt.ball_radius, Position{}, Velocity{}, opt.ball_init_visible));

	boards_.emplace_back(std::make_shared<ObjBoard>());
	boards_.emplace_back(std::make_shared<ObjBoard>());
	game_state_ = GameState::WAIT_TO_START;
}

void Playground::init_gameobjs()
{
	//balls_[0] = std::make_shared<ObjBall>(opt.ball_radius, Position{}, Velocity{}, opt.ball_init_visible);
	//boards_[0] = std::make_shared<ObjBoard>(opt.board_length, opt.board_width, Position{ opt.board_margin, opt.height / 2.0f });
	//boards_[1] = std::make_shared<ObjBoard>(opt.board_length, opt.board_width, Position{ opt.width - 1 - opt.board_margin, opt.height / 2.0f });
	balls_[0]->set_pos({});
	balls_[0]->set_vel({});
	balls_[0]->set_visible(opt.ball_init_visible);

	boards_[0]->set_pos(Position(opt.board_margin, opt.height / 2.0f));
	boards_[0]->set_vel({});
	boards_[0]->set_visible(true);

	boards_[1]->set_pos(Position(opt.width - 1 - opt.board_margin, opt.height / 2.0f));
	boards_[1]->set_vel({});
	boards_[1]->set_visible(true);

}

void Playground::init_gamestate()
{
	game_state_ = GameState::WAIT_TO_START;
}

void Playground::board_ctrl(int player, int dir)
{
	//qDebug("board ctrl: %d %d", player, dir);
	if (player == 0) {
		if (dir == 0) {
			boards_[0]->set_vel(Velocity{ 0, -opt.board_speed });
		}
		else if (dir == 1) {
			boards_[0]->set_vel(Velocity{ 0, opt.board_speed });
		}
		else {
			// 速度归零
			boards_[0]->set_vel(Velocity{ 0, 0 });
		}
	}
	else {
		if (dir == 0) {
			boards_[1]->set_vel(Velocity{ 0, -opt.board_speed });
		}
		else if (dir == 1) {
			boards_[1]->set_vel(Velocity{ 0, opt.board_speed });
		}
		else {
			// 速度归零
			boards_[1]->set_vel(Velocity{ 0,0 });
		}
	}
}

void Playground::shoot_ball(int player, float angle)
{
	qDebug() << "shoot a ball";
	std::shared_ptr<ObjBall>& ball = balls_[0];
	//angle = angle / 180.0f * M_PI;
	//qDebug() << __FUNCTION__ << "angle:" << angle;
	if (player == 0) {
		// 左边
		ball->set_pos(Position(left_board()->pos));
		ball->set_vel(Velocity(
			opt.ball_speed * cosf(angle),
			opt.ball_speed * sinf(angle)
		));
	}
	else {
		ball->set_pos(Position(right_board()->pos));
		ball->set_vel(Velocity(
			opt.ball_speed * cosf(angle),
			opt.ball_speed * sinf(angle)
		));
	}
	ball->visible = true;
}

void Playground::add_acceleration(MovingObj* obj, const Vec2D& a)
{
	obj->a += a;
}

void Playground::apply_acceleration(MovingObj* obj, const Vec2D& a)
{
	obj->a = a;
}

void Playground::update_one_frame()
{
	for (auto& obj : get_game_objs()) {
		obj->update_one_frame(opt.delta_t);
	}
	board_check_bound();
	ball_check_bound();
	GameState st = game_state();
	if (st == GameState::CHECKOUT) {
		// 结算后初始化物体
		init_gameobjs();
		emit a_new_round();
		game_state_ = GameState::WAIT_TO_START;
	}
}

std::vector<std::shared_ptr<GameObj>> Playground::get_game_objs()
{
	return std::vector<std::shared_ptr<GameObj>>{boards_[0], boards_[1], balls_[0]};
}

void Playground::board_check_bound()
{
	const std::shared_ptr<ObjBoard>& b1 = left_board();
	const std::shared_ptr<ObjBoard>& b2 = right_board();
	float b1_half_length = b1->half_length;
	float b2_half_length = b2->half_length;
	if (b1->pos.y < b1_half_length) {
		b1->pos.y = b1_half_length;
	}
	else if (b1->pos.y + b1_half_length > opt.height - 1) {
		b1->pos.y = opt.height - 1 - b1_half_length;
	}

	if (b2->pos.y < b2_half_length) {
		b2->pos.y = b2_half_length;
	}
	else if (b2->pos.y + b2_half_length > opt.height - 1) {
		b2->pos.y = opt.height - 1 - b2_half_length;
	}
}

void Playground::ball_check_bound()
{
	// 线性插值计算碰撞点和碰撞后坐标
	auto calc_t = [](float hit, float origin, float deltat, float vel) { return (hit - origin) / (deltat * vel); };
	auto calc_new = [](float hit, float origin, float deltat, float vel) { return 2 * hit - deltat * vel - origin; };
	for (auto&& ball : balls_) {
		Velocity& vel = ball->vel;
		Position& pos = ball->pos;
		const Position& pre_pos = ball->pre_pos;
		const float radius = ball->radius;
		auto&& pos_b1 = left_board()->pos;
		auto&& pos_b2 = right_board()->pos;
		auto b1_half_width = left_board()->half_width;
		auto b1_half_length = left_board()->half_length;
		auto b2_half_width = right_board()->half_width;
		auto b2_half_length = right_board()->half_length;

		float b1_left_bound = pos_b1.x - b1_half_width;
		float b1_right_bound = pos_b1.x + b1_half_width;
		float b1_up_bound = pos_b1.y - b1_half_length;
		float b1_down_bound = pos_b1.y + b1_half_length;

		float b2_left_bound = pos_b2.x - b2_half_width;
		float b2_right_bound = pos_b2.x + b2_half_width;
		float b2_up_bound = pos_b2.y - b2_half_length;
		float b2_down_bound = pos_b2.y + b2_half_length;
		/* 检查上下边界 */
		if (vel.y > 0) {
			const float hit_y = opt.height - 1;
			const float t = calc_t(hit_y, pre_pos.y + radius, opt.delta_t, vel.y);
			if (t <= 1 && 0 <= t) { // 碰撞
				ball->pos.y = calc_new(hit_y, pre_pos.y + radius, opt.delta_t, vel.y) - radius;
				ball->reverse_y_vel();
			}
		}
		else if (vel.y < 0) {
			const float hit_y = 0;
			const float t = calc_t(hit_y, pre_pos.y - radius, opt.delta_t, vel.y);
			if (t <= 1 && 0 <= t) { // 碰撞
				ball->pos.y = calc_new(hit_y, pre_pos.y - radius, opt.delta_t, vel.y) + radius;
				ball->reverse_y_vel();
			}
		}

		/****** 检查板子碰撞 *****/
		if (vel.x > 0) {
			// 检查右板子
			float hit_x = pos_b2.x - b2_half_width;
			if (b2_down_bound + radius >= pos.y && pos.y >= b2_up_bound - radius) {
				const float t = calc_t(hit_x, pre_pos.x + radius, opt.delta_t, vel.x);
				if (t <= 1 && 0 <= t) { // 碰撞
					ball->pos.x = calc_new(hit_x, pre_pos.x + radius, opt.delta_t, vel.x) - radius;
					ball->reverse_x_vel();
				}
			}
		}
		else if (vel.x < 0) {
			// 检查左板子
			const float hit_x = pos_b1.x + b1_half_width;
			if (b1_down_bound + radius >= pos.y && pos.y >= b1_up_bound - radius) {
				const float t = calc_t(hit_x, ball->pos.x - radius, opt.delta_t, vel.x);
				if (t <= 1 && 0 <= t) { // 碰撞
					ball->pos.x = calc_new(hit_x, ball->pos.x - radius, opt.delta_t, vel.x) + radius;
					ball->reverse_x_vel();
				}
			}
		}

		if (vel.y > 0) {
			// 两个板子的上边界
			bool judge_up_bound = true;
			float up_bound;
			if (b1_left_bound - radius <= pos.x && pos.x <= b1_right_bound + radius) { // 球位于左边板子的范围
				up_bound = b1_up_bound;
			}
			else if (b2_left_bound - radius <= pos.x && pos.x <= b2_right_bound + radius) {
				up_bound = b2_up_bound;
			}
			else {
				judge_up_bound = false;
			}
			if (judge_up_bound) {
				float t = calc_t(up_bound, pre_pos.y + radius, opt.delta_t, vel.y);
				if (t <= 1 && 0 <= t) { // 碰撞
					ball->pos.y = calc_new(up_bound, pre_pos.y + radius, opt.delta_t, vel.y) - radius;
					ball->reverse_y_vel();
				}
			}
		}
		else if (vel.y < 0) {
			// 两个板子的下边界
			bool judge_down_bound = true;
			float down_bound;
			if (b1_left_bound - radius <= pos.x && pos.x <= b1_right_bound + radius) { // 球位于左边板子的范围
				down_bound = b1_down_bound;
			}
			else if (b2_left_bound - radius <= pos.x && pos.x <= b2_right_bound + radius) {
				down_bound = b2_down_bound;
			}
			else {
				judge_down_bound = false;
			}
			if (judge_down_bound) {
				float t = calc_t(down_bound, pre_pos.y - radius, opt.delta_t, vel.y);
				if (t <= 1 && 0 <= t) { // 碰撞
					ball->pos.y = calc_new(down_bound, pre_pos.y - radius, opt.delta_t, vel.y) + radius;
					ball->reverse_y_vel();
				}
			}

		}

		// 判断是否胜利
		if (pos.x > opt.width) { // 左赢
			player_win(0);
			game_state_ = CHECKOUT;
			break;
		}
		else if (pos.x < 0) { //右赢
			player_win(1);
			game_state_ = CHECKOUT;
			break;
		}
	}

}

void Playground::player_win(int player)
{
	if (player == 0) {
		possession_ = RIGHT;
		score_p1++;
	}
	else if (player == 1) {
		possession_ = LEFT;
		score_p2++;
	}
}
