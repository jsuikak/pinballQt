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
	balls_[0] = std::make_shared<ObjBall>(opt.ball_radius, Position{}, Velocity{}, opt.ball_init_visible);
	boards_[0] = std::make_shared<ObjBoard>(opt.board_length, opt.board_width, Position{ opt.board_margin, opt.height / 2.0f });
	boards_[1] = std::make_shared<ObjBoard>(opt.board_length, opt.board_width, Position{ opt.width - 1 - opt.board_margin, opt.height / 2.0f });
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
	if (player == 0) {
		// 左边
		balls_[0]->set_pos(Position(boards_[0]->pos));
		balls_[0]->set_vel(Velocity(
			opt.ball_speed * cosf(angle),
			opt.ball_speed * sinf(angle)
		));
	}
	else {
		balls_[0]->set_pos(Position(boards_[1]->pos));
		balls_[0]->set_vel(Velocity(
			opt.ball_speed * cosf(180 - angle),
			opt.ball_speed * sinf(180 - angle)
		));
	}

}

RC Playground::update_one_frame()
{
	for (auto& obj : get_game_objs()) {
		obj->update_one_frame(opt.delta_t);
	}
	board_check_bound();
	ball_check_bound();

	return RC::SUCCESS;
}

std::vector<std::shared_ptr<GameObj>> Playground::get_game_objs()
{
	return std::vector<std::shared_ptr<GameObj>>{boards_[0], boards_[1], balls_[0]};
}

void Playground::board_check_bound()
{
	std::shared_ptr<ObjBoard> b1 = boards_[0], b2 = boards_[1];
	float b1_half_length = b1->length / 2.0f;
	float b2_half_length = b2->length / 2.0f;
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
			std::shared_ptr<ObjBoard> board = boards_[1];
			float board_up_bound = board->pos.y - board->length / 2.0f;
			float board_down_bound = board->pos.y + board->length / 2.0f;
			float board_half_width = board->width / 2.0f;
			float board_left_bound = board->pos.x - board_half_width;
			float board_right_bound = board->pos.x + board_half_width;
			float hit_x = board->pos.x - board_half_width;
			auto t = calc_t(hit_x, pre_pos.x + radius, opt.delta_t, vel.x);
			if (t <= 1 && 0 <= t) { // 碰撞
				//qDebug() << "hit board left bound line";
				if (board_down_bound + radius >= pos.y && pos.y >= board_up_bound - radius) {
					//qDebug() << "hit board left bound";
					float new_x = calc_new(hit_x, pre_pos.x + radius, opt.delta_t, vel.x);
					ball->pos.x = new_x - radius;
					ball->reverse_x_vel();
				}
			}
			// 右板子上下边界
			if (vel.y > 0) {
				// 上边界
				float t = calc_t(board_up_bound, pre_pos.y + radius, opt.delta_t, vel.y);
				if (t <= 1 && 0 <= t) { // 碰撞
					if (board_left_bound - radius <= pos.x && pos.x <= board_right_bound + radius) {
						//qDebug() << "hit board up bound";
						ball->pos.y = calc_new(board_up_bound, pre_pos.y + radius, opt.delta_t, vel.y) - radius;
						ball->reverse_y_vel();
					}
				}
			}
			else if (vel.y < 0) {
				// 下边界
				float t = calc_t(board_down_bound, pre_pos.y - radius, opt.delta_t, vel.y);
				if (t <= 1 && 0 <= t) { // 碰撞
					if (board_left_bound - radius <= pos.x && pos.x <= board_right_bound + radius) {
						//qDebug() << "hit board down bound";
						ball->pos.y = calc_new(board_down_bound, pre_pos.y - radius, opt.delta_t, vel.y) + radius;
						ball->reverse_y_vel();
					}
				}
			}
		}
		else if (vel.x < 0) {
			// 检查左板子
			std::shared_ptr<ObjBoard> board = boards_[0];
			const float board_up_bound = board->pos.y - board->length / 2.0f;
			const float board_down_bound = board->pos.y + board->length / 2.0f;
			const float board_half_width = board->width / 2.0f;
			const float board_left_bound = board->pos.x - board_half_width;
			const float board_right_bound = board->pos.x + board_half_width;
			const float hit_x = board->pos.x + board_half_width;
			const float t = calc_t(hit_x, ball->pos.x - radius, opt.delta_t, vel.x);
			if (t <= 1 && 0 <= t) { // 碰撞
				//qDebug() << "hit board right bound line";
				if (board_down_bound + radius >= pos.y && pos.y >= board_up_bound - radius) {
					//qDebug() << "hit board right bound";
					float new_x = calc_new(hit_x, ball->pos.x - radius, opt.delta_t, vel.x);
					ball->pos.x = new_x + radius;
					ball->reverse_x_vel();
				}
			}
			// 左板子上下边界
			if (vel.y > 0) {
				// 上边界
				float t = calc_t(board_up_bound, pre_pos.y + radius, opt.delta_t, vel.y);
				if (t <= 1 && 0 <= t) { // 碰撞
					if (board_left_bound - radius <= pos.x && pos.x <= board_right_bound + radius) {
						//qDebug() << "hit board up bound";
						ball->pos.y = calc_new(board_up_bound, pre_pos.y + radius, opt.delta_t, vel.y) - radius;
						ball->reverse_y_vel();
					}
				}
			}
			else if (vel.y < 0) {
				// 下边界
				float t = calc_t(board_down_bound, pre_pos.y - radius, opt.delta_t, vel.y);
				if (t <= 1 && 0 <= t) { // 碰撞
					if (board_left_bound - radius <= pos.x && pos.x <= board_right_bound + radius) {
						//qDebug() << "hit board down bound";
						ball->pos.y = calc_new(board_down_bound, pre_pos.y - radius, opt.delta_t, vel.y) + radius;
						ball->reverse_y_vel();
					}
				}
			}
		}
	}

}
