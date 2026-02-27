#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::Size _visibleSize;
    int _ballCounter;

    static const int PHYSICS_CATEGORY_BALL = 0x01;
    static const int PHYSICS_CATEGORY_EDGE = 0x02;
    static const int PHYSICS_CATEGORY_ALL = 0xFFFFFFFF;

public:
    static cocos2d::Scene *createScene();

    virtual bool init();
    virtual void onEnter();

    CREATE_FUNC(HelloWorld);

    void addEdgeBox();
    void addBall(cocos2d::Vec2 position);

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif  // __HELLOWORLD_SCENE_H__
