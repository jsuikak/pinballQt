#pragma once

// 游戏内物体
enum ObjType
{
	NONE,
	BALL,
	BOARD,
};

class GameObj
{
public:
	GameObj(const ObjType& type = NONE, bool visible = true)
		: type(type), visible(visible) {}
	virtual ~GameObj() {};
	//virtual void update_one_frame() = 0;
	virtual void update_one_frame(float delta_t) = 0;
	ObjType type;
	bool visible;

};

