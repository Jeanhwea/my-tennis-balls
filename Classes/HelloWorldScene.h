#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::Size _visibleSize;
    int _ballCounter = 0;

    // Physics category bitmasks for collision filtering
    static constexpr int PHYSICS_CATEGORY_BALL = 0x01;
    static constexpr int PHYSICS_CATEGORY_EDGE = 0x02;
    static constexpr int PHYSICS_CATEGORY_ALL  = 0xFFFFFFFF;

    // Physics tuning constants
    static constexpr float GRAVITY_Y            = -98.0f;
    static constexpr float BALL_SCALE           = 0.1f;
    static constexpr float BALL_SPRITE_PADDING  = 18.0f;  // padding between sprite edge and physics circle
    static constexpr float BALL_DENSITY         = 0.1f;
    static constexpr float BALL_RESTITUTION     = 0.85f;
    static constexpr float BALL_FRICTION        = 0.0f;
    static constexpr float EDGE_RESTITUTION     = 1.0f;
    static constexpr float EDGE_FRICTION        = 0.0f;

public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void onEnter() override;

    CREATE_FUNC(HelloWorld);

private:
    void addEdgeBox();
    void addBall(const cocos2d::Vec2 &position);

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif  // __HELLOWORLD_SCENE_H__
