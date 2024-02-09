#include "Playground.h"
#include <qdebug.h>

Playground::Playground(GameOpt opt) :opt(opt), width_(opt.width), height_(opt.height)
{
	balls_.emplace_back(std::make_shared<ObjBall>(Position{}, Velocity{}, opt.balll_init_visible)); // 初始化为不可见

	boards_.emplace_back(std::make_shared<ObjBoard>());
	boards_.emplace_back(std::make_shared<ObjBoard>());
}

void Playground::init_gameobjs()
{
	balls_[0] = std::make_shared<ObjBall>(Position{}, Velocity{}, opt.balll_init_visible);// 初始化为不可见
	boards_[0] = std::make_shared<ObjBoard>(Position{ (float)board_margin_, height_ / 2.0f });
	boards_[1] = std::make_shared<ObjBoard>(Position{ (float)width_ - 1 - (float)board_margin_, height_ / 2.0f });
}

void Playground::board_ctrl(int player, int dir)
{
	qDebug("boardd ctrl: %d %d", player, dir);
	if (player == 0) {
		if (dir == 0) {
			boards_[0]->set_vel(Velocity{ 0, -board_speed_ });
		}
		else if (dir == 1) {
			boards_[0]->set_vel(Velocity{ 0, board_speed_ });
		}
		else {
			// 速度归零
			boards_[0]->set_vel(Velocity{ 0, 0 });
		}
	}
	else {
		if (dir == 0) {
			boards_[1]->set_vel(Velocity{ 0, -board_speed_ });
		}
		else if (dir == 1) {
			boards_[1]->set_vel(Velocity{ 0, board_speed_ });
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
			ball_speed * cosf(angle),
			ball_speed * sinf(angle)
		));
	}
	else {

	}

}

RC Playground::update_one_frame()
{
	for (auto& obj : get_game_objs()) {
		obj->update_one_frame();
	}
	board_check_bound();
	ball_check_bound();

	return RC::SUCCESS;
}

std::vector<std::shared_ptr<GameObj>> Playground::get_game_objs()
{
	return std::vector<std::shared_ptr<GameObj>>{boards_[0], boards_[1], balls_[0]};
}

RC Playground::game_status()
{
	return RC::SUCCESS;
}

void Playground::board_check_bound()
{
	std::shared_ptr<ObjBoard> b1 = boards_[0], b2 = boards_[1];
	float b1_half_length = (float)b1->length / 2.0f;
	float b2_half_length = (float)b2->length / 2.0f;
	if (b1->pos.y < b1_half_length) {
		b1->pos.y = b1_half_length;
	}
	else if (b1->pos.y + b1_half_length > this->height_ - 1) {
		b1->pos.y = this->height_ - 1 - b1_half_length;
	}

	if (b2->pos.y < b2_half_length) {
		b2->pos.y = b2_half_length;
	}
	else if (b2->pos.y + b2_half_length > this->height_ - 1) {
		b2->pos.y = this->height_ - 1 - b2_half_length;
	}
}

void Playground::ball_check_bound()
{
	for (auto&& ball : balls_) {
		auto&& vel = ball->vel;
		auto&& pos = ball->pos;
		const float radius = ball->radius;
		/* 检查上下边界 */
		if (vel.y > 0 && (pos.y + radius > height_ - 1)) {
			qDebug() << "ball check up bound";
			vel.reverse_y();
		}
		else if (vel.y < 0 && (pos.y < radius)) {
			qDebug() << "ball check down bound";
			vel.reverse_y();
		}

		/* 检查板子碰撞 */
		if (vel.x > 0) {
			// 检查右板子
			auto&& board = boards_[1];
			float board_up_bound = board->pos.y - board->length / 2.0f;
			float board_down_bound = board->pos.y + board->length / 2.0f;
			if (pos.x + radius > board->pos.x - board->width / 2.0f)
			{
				//qDebug() << "ball hit right line";
				if (board_down_bound >= pos.y && pos.y >= board_up_bound) {
					//qDebug() << "ball hit right board";
					vel.reverse_x();
				}
			}
		}
		else if (vel.x < 0) {
			// 检查左板子
			auto&& board = boards_[0];
			float board_up_bound = board->pos.y - board->length / 2.0f;
			float board_down_bound = board->pos.y + board->length / 2.0f;
			if (pos.x - radius < board->pos.x + board->width / 2.0f)
			{
				//qDebug() << "ball hit left line";
				if (board_down_bound >= pos.y && pos.y >= board_up_bound) {
					//qDebug() << "ball hit left board";
					vel.reverse_x();
				}
			}
		}
	}

}
