#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

/// @file GameConstants.h
/// Global constants: tags, masks, physics parameters, layout ratios, scoring rules.

// -- Fonts --
constexpr const char *FONT_UI = "fonts/arial.ttf";
constexpr const char *FONT_TITLE = "fonts/Marker Felt.ttf";

// -- Node Tags --
constexpr int TAG_BALL = 100;
constexpr int TAG_TARGET = 200;
constexpr int TAG_FLOOR = 300;
constexpr int TAG_TRAY = 400;
constexpr int TAG_OBSTACLE = 500;

// -- Collision Category Masks --
constexpr int CATEGORY_BALL = 0x01;
constexpr int CATEGORY_EDGE = 0x02;
constexpr int CATEGORY_TARGET = 0x04;
constexpr int CATEGORY_FLOOR = 0x08;
constexpr int CATEGORY_TRAY = 0x10;
constexpr int CATEGORY_OBSTACLE = 0x20;
constexpr int CATEGORY_ALL = 0xFFFFFFFF;

// -- Global Physics --
constexpr float GRAVITY_Y = -980.0f;

// -- Ball --
constexpr float BALL_SCALE = 0.22f;
constexpr float BALL_SPRITE_PADDING = 18.0f;
constexpr float BALL_DENSITY = 1.0f;
constexpr float BALL_RESTITUTION = 0.8f;
constexpr float BALL_FRICTION = 0.3f;

// -- Ball Lighting --
constexpr float BALL_SHADOW_OFFSET_Y = -8.0f;  // Shadow Y offset

// -- Launch Parameters --
constexpr float LAUNCH_FORCE_SCALE = 5.5f;
constexpr float MAX_LAUNCH_SPEED = 1500.0f;
constexpr float MIN_DRAG_DISTANCE = 20.0f;

// -- Target Ball --
constexpr float TARGET_SCALE = 0.18f;
constexpr float TARGET_DENSITY = 0.8f;
constexpr float TARGET_RESTITUTION = 0.5f;
constexpr float TARGET_FRICTION = 0.6f;

// -- Edge Walls --
constexpr float EDGE_RESTITUTION = 0.9f;
constexpr float EDGE_FRICTION = 0.2f;

// -- Tray --
constexpr float TRAY_X = 0.15f;
constexpr float TRAY_Y = 0.45f;
constexpr float TRAY_WIDTH_RATIO = 0.22f;
constexpr float TRAY_THICKNESS = 8.0f;
constexpr float TRAY_DENSITY = 1.0f;
constexpr float TRAY_RESTITUTION = 0.2f;
constexpr float TRAY_FRICTION = 0.8f;

// -- Obstacle --
constexpr float OBSTACLE_MIN_SIZE = 30.0f;
constexpr float OBSTACLE_MAX_SIZE = 80.0f;
constexpr float OBSTACLE_ZONE_LEFT = 0.05f;
constexpr float OBSTACLE_ZONE_RIGHT = 0.55f;
constexpr float OBSTACLE_ZONE_BOTTOM = 0.10f;
constexpr float OBSTACLE_ZONE_TOP = 0.40f;
constexpr float OBSTACLE_DENSITY = 1.0f;
constexpr float OBSTACLE_RESTITUTION = 1.0f;
constexpr float OBSTACLE_FRICTION = 0.0f;

// -- Area Layout --
constexpr float LAUNCH_ZONE_RATIO = 0.20f;
constexpr float DIVIDER_THICKNESS = 2.0f;
constexpr float FLOOR_HEIGHT = 10.0f;

// -- Scoring --
constexpr float COMBO_TIMEOUT = 2.0f;
constexpr int SCORE_PER_HIT = 10;
constexpr int SCORE_TARGET_FALL = 50;

// -- Out-of-Bounds Thresholds --
constexpr float OOB_BOTTOM = -50.0f;
constexpr float OOB_TOP_MARGIN = 200.0f;
constexpr float OOB_SIDE_MARGIN = 200.0f;

#endif  // __GAME_CONSTANTS_H__
