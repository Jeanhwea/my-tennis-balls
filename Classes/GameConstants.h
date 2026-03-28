#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

// Node tags
constexpr int TAG_BALL = 100;
constexpr int TAG_TARGET = 200;
constexpr int TAG_FLOOR = 300;
constexpr int TAG_TRAY = 400;

// Physics category bitmasks
constexpr int CATEGORY_BALL = 0x01;
constexpr int CATEGORY_EDGE = 0x02;
constexpr int CATEGORY_TARGET = 0x04;
constexpr int CATEGORY_FLOOR = 0x08;
constexpr int CATEGORY_TRAY = 0x10;
constexpr int CATEGORY_ALL = 0xFFFFFFFF;

// Physics tuning
constexpr float GRAVITY_Y = -980.0f;
constexpr float BALL_SCALE = 0.1f;
constexpr float BALL_SPRITE_PADDING = 18.0f;
constexpr float BALL_DENSITY = 1.0f;
constexpr float BALL_RESTITUTION = 0.8f;
constexpr float BALL_FRICTION = 0.3f;
constexpr float EDGE_RESTITUTION = 0.9f;
constexpr float EDGE_FRICTION = 0.2f;

// Launch tuning
constexpr float LAUNCH_FORCE_SCALE = 3.0f;
constexpr float MAX_LAUNCH_SPEED = 2000.0f;
constexpr float MIN_DRAG_DISTANCE = 20.0f;

// Gameplay
constexpr float COMBO_TIMEOUT = 2.0f;
constexpr int SCORE_PER_HIT = 10;
constexpr int SCORE_TARGET_FALL = 50;
constexpr int MAX_BALLS = 20;
constexpr float FLOOR_HEIGHT = 10.0f;
constexpr int TARGET_COUNT = 5;

// Zone layout
constexpr float LAUNCH_ZONE_RATIO = 0.20f;  // right 20%

// Tray layout
constexpr float TRAY_X = 0.15f;            // tray center X (ratio of width)
constexpr float TRAY_Y = 0.45f;            // tray Y position (ratio of height)
constexpr float TRAY_WIDTH_RATIO = 0.22f;  // tray width (ratio of width)
constexpr float TRAY_THICKNESS = 8.0f;

// Target ball on tray
constexpr float TARGET_SCALE = 0.08f;
constexpr float TARGET_DENSITY = 0.8f;
constexpr float TARGET_RESTITUTION = 0.5f;
constexpr float TARGET_FRICTION = 0.6f;

// Divider line
constexpr float DIVIDER_THICKNESS = 2.0f;

#endif  // __GAME_CONSTANTS_H__
