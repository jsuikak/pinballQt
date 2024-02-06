#include "Position.h"

Position Position::up_pos(float n)
{
	return Position(x, y - n);
}

Position Position::down_pos(float n)
{
	return Position(x, y + n);
}

Position Position::left_pos(float n)
{
	return Position(x - n, y);
}

Position Position::right_pos(float n)
{
	return Position(x + n, y);
}
