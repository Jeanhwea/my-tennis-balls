#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

// Node tags for identification
constexpr int TAG_BALL = 100;
constexpr int TAG_OBSTACLE = 200;
constexpr int TAG_FLOOR = 300;

// Physics category bitmasks
constexpr int CATEGORY_BALL = 0x01;
constexpr int CATEGORY_EDGE = 0x02;
constexpr int CATEGORY_OBSTACLE = 0x04;
constexpr int CATEGORY_FLOOR = 0x08;
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
constexpr int MAX_BALLS = 30;
constexpr float FLOOR_HEIGHT = 10.0f;
constexpr int OBSTACLE_COUNT = 6;

// Obstacle sizing
constexpr float OBSTACLE_MIN_SIZE = 40.0f;
constexpr float OBSTACLE_MAX_SIZE = 100.0f;

// Zone layout (ratios of screen dimensions)
constexpr float LAUNCH_ZONE_RATIO = 0.20f;     // right 20% of screen width
constexpr float OBSTACLE_ZONE_LEFT = 0.05f;    // obstacles start at 5% width
constexpr float OBSTACLE_ZONE_RIGHT = 0.75f;   // obstacles end at 75% width
constexpr float OBSTACLE_ZONE_BOTTOM = 0.10f;  // obstacles start at 10% height
constexpr float OBSTACLE_ZONE_TOP = 0.85f;     // obstacles end at 85% height

// Divider line
constexpr float DIVIDER_THICKNESS = 2.0f;

#endif  // __GAME_CONSTANTS_H__
