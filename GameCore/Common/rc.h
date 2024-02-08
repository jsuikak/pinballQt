#pragma once
/**
 * @brief 这个文件定义函数返回码/错误码(Return Code)
 * @enum RC
 */
#define DEFINE_RCS \
    DEFINE_RC(SUCCESS)\
    DEFINE_RC(NORMAL_EXIT)\
    DEFINE_RC(BALL_REACH_LEFT_BOUND)\
    DEFINE_RC(BALL_REACH_RIGHT_BOUND)\
    DEFINE_RC(GAME_OVER)\
    DEFINE_RC(GAME_RESTART)\

enum class RC {
#define DEFINE_RC(name) name,
    DEFINE_RCS
#undef DEFINE_RC
};

extern const char* strrc(RC rc);
