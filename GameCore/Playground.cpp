#include "Playground.h"

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
}
