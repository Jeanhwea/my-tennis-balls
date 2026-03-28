#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    int _ballCounter = 0;
    int _score       = 0;
    int _combo       = 0;
    float _comboTimer = 0.0f;

    // UI elements
    cocos2d::Label *_scoreLabel = nullptr;
    cocos2d::Label *_comboLabel = nullptr;
    cocos2d::Label *_ballCountLabel = nullptr;
    cocos2d::Label *_hintLabel = nullptr;

    // Drag-to-launch state
    bool _isDragging = false;
    cocos2d::Vec2 _dragStart;
    cocos2d::DrawNode *_aimLine = nullptr;

    // Physics category bitmasks for collision filtering
    static constexpr int CATEGORY_BALL     = 0x01;
    static constexpr int CATEGORY_EDGE     = 0x02;
    static constexpr int CATEGORY_OBSTACLE = 0x04;
    static constexpr int CATEGORY_FLOOR    = 0x08;
    static constexpr int CATEGORY_ALL      = 0xFFFFFFFF;

    // Physics tuning constants
    static constexpr float GRAVITY_Y           = -980.0f;
    static constexpr float BALL_SCALE          = 0.1f;
    static constexpr float BALL_SPRITE_PADDING = 18.0f;
    static constexpr float BALL_DENSITY        = 1.0f;
    static constexpr float BALL_RESTITUTION    = 0.8f;
    static constexpr float BALL_FRICTION       = 0.3f;
    static constexpr float EDGE_RESTITUTION    = 0.9f;
    static constexpr float EDGE_FRICTION       = 0.2f;

    // Launch tuning
    static constexpr float LAUNCH_FORCE_SCALE  = 3.0f;
    static constexpr float MAX_LAUNCH_SPEED    = 2000.0f;
    static constexpr float MIN_DRAG_DISTANCE   = 20.0f;

    // Gameplay
    static constexpr float COMBO_TIMEOUT       = 2.0f;
    static constexpr int SCORE_PER_HIT         = 10;
    static constexpr int MAX_BALLS             = 30;
    static constexpr float FLOOR_HEIGHT        = 10.0f;
    static constexpr int OBSTACLE_COUNT        = 6;

public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void onEnter() override;
    void update(float dt) override;

    CREATE_FUNC(HelloWorld);

private:
    // Setup
    void addEdgeWalls();
    void addFloorSensor();
    void addObstacles();
    void addHUD();

    // Gameplay
    void addBall(const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity);
    void removeBall(cocos2d::Node *ball);
    void addScore(int points);
    void resetCombo();
    void updateHUD();
    void spawnObstacle(const cocos2d::Vec2 &position, bool isCircle);

    // Visual effects
    cocos2d::Color3B randomBallColor() const;
    void flashNode(cocos2d::Node *node);
    void spawnHitParticle(const cocos2d::Vec2 &position);
    void showFloatingScore(const cocos2d::Vec2 &position, int points);

    // Input
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

    // Physics callbacks
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif  // __HELLOWORLD_SCENE_H__
