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
	boards_[1] = std::make_shared<ObjBoard>(Position{ (float)board_margin_, height_ / 2.0f });
}

std::vector<std::shared_ptr<GameObj>> Playground::get_game_objs()
{
	return std::vector<std::shared_ptr<GameObj>>{boards_[0], boards_[1], balls_[0]};
}

RC Playground::game_status()
{
	return RC::SUCCESS;
}
