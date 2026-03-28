#ifndef __BALL_MANAGER_H__
#define __BALL_MANAGER_H__

#include <functional>

#include "cocos2d.h"

class BallManager
{
public:
    using CountChangeCallback = std::function<void(int ballCount)>;

    explicit BallManager(cocos2d::Node *parent);

    void setOnCountChange(CountChangeCallback cb) { _onCountChange = std::move(cb); }

    void addBall(const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity);
    void removeBall(cocos2d::Node *ball);
    int countBalls() const;

private:
    cocos2d::Node *_parent = nullptr;
    int _ballCounter = 0;
    CountChangeCallback _onCountChange;

    static cocos2d::Color3B randomColor();
    void notifyCountChange();
};

#endif  // __BALL_MANAGER_H__
