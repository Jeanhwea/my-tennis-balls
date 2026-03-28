#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <memory>

#include "BallManager.h"
#include "HUD.h"
#include "ScoreManager.h"
#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void onEnter() override;
    void update(float dt) override;

    CREATE_FUNC(HelloWorld);

private:
    cocos2d::Size _visibleSize;

    // Sub-systems
    std::unique_ptr<BallManager> _ballMgr;
    ScoreManager _scoreMgr;
    HUD *_hud = nullptr;

    // Drag-to-launch state
    bool _isDragging = false;
    cocos2d::Vec2 _dragStart;
    cocos2d::DrawNode *_aimLine = nullptr;

    // Setup
    void addEdgeWalls();
    void addFloorSensor();
    void refreshHUD();

    // Input
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

    // Physics
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif  // __HELLOWORLD_SCENE_H__
