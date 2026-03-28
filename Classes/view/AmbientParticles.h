#ifndef __AMBIENT_PARTICLES_H__
#define __AMBIENT_PARTICLES_H__

#include "cocos2d.h"

/// 背景浮动光点，营造深空氛围。
class AmbientParticles : public cocos2d::Node
{
public:
    static AmbientParticles *create(const cocos2d::Size &area);

    void start();
    void stop();

private:
    bool initWithArea(const cocos2d::Size &area);
    void spawnDot();

    cocos2d::Size _area;
    bool _running = false;
};

#endif  // __AMBIENT_PARTICLES_H__
