#pragma once
// 游戏进行的场地
class Playground
{
public:
	void update_all(); //更新场地里所有物体(一帧)

private:
	// 场地尺寸
	int width_;
	int height_;
};

