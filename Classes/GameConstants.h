#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

// 字体
constexpr const char *FONT_UI = "fonts/arial.ttf";
constexpr const char *FONT_TITLE = "fonts/Marker Felt.ttf";

// 节点标签
constexpr int TAG_BALL = 100;
constexpr int TAG_TARGET = 200;
constexpr int TAG_FLOOR = 300;
constexpr int TAG_TRAY = 400;

// 物理碰撞分类掩码
constexpr int CATEGORY_BALL = 0x01;
constexpr int CATEGORY_EDGE = 0x02;
constexpr int CATEGORY_TARGET = 0x04;
constexpr int CATEGORY_FLOOR = 0x08;
constexpr int CATEGORY_TRAY = 0x10;
constexpr int CATEGORY_ALL = 0xFFFFFFFF;

// 物理参数调优
constexpr float GRAVITY_Y = -980.0f;
constexpr float BALL_SCALE = 0.1f;
constexpr float BALL_SPRITE_PADDING = 18.0f;
constexpr float BALL_DENSITY = 1.0f;
constexpr float BALL_RESTITUTION = 0.8f;
constexpr float BALL_FRICTION = 0.3f;
constexpr float EDGE_RESTITUTION = 0.9f;
constexpr float EDGE_FRICTION = 0.2f;

// 发射参数调优
constexpr float LAUNCH_FORCE_SCALE = 2.5f;
constexpr float MAX_LAUNCH_SPEED = 1500.0f;
constexpr float MIN_DRAG_DISTANCE = 20.0f;

// 游戏玩法
constexpr float COMBO_TIMEOUT = 2.0f;
constexpr int SCORE_PER_HIT = 10;
constexpr int SCORE_TARGET_FALL = 50;
constexpr float FLOOR_HEIGHT = 10.0f;

// 区域布局
constexpr float LAUNCH_ZONE_RATIO = 0.20f;  // 右侧 20%

// 托盘布局
constexpr float TRAY_X = 0.15f;            // 托盘中心 X（宽度比例）
constexpr float TRAY_Y = 0.45f;            // 托盘 Y 位置（高度比例）
constexpr float TRAY_WIDTH_RATIO = 0.22f;  // 托盘宽度（宽度比例）
constexpr float TRAY_THICKNESS = 8.0f;

// 托盘上的目标球
constexpr float TARGET_SCALE = 0.08f;
constexpr float TARGET_DENSITY = 0.8f;
constexpr float TARGET_RESTITUTION = 0.5f;
constexpr float TARGET_FRICTION = 0.6f;

// 分隔线
constexpr float DIVIDER_THICKNESS = 2.0f;

#endif  // __GAME_CONSTANTS_H__
