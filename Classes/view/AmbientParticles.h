/// @file AmbientParticles.h
/// 背景浮动光点粒子系统，营造深空氛围。支持动态质量调节以控制粒子数量。

#ifndef __AMBIENT_PARTICLES_H__
#define __AMBIENT_PARTICLES_H__

#include "cocos2d.h"

/// @class AmbientParticles
/// 背景浮动光点，营造深空氛围。
class AmbientParticles : public cocos2d::Node
{
public:
    static AmbientParticles *create(const cocos2d::Size &area);

    void start();
    void stop();
    void pause();
    void resume();

    // Dynamic quality adjustment
    void setQualityLevel(int level);  // 0=off, 1=low, 2=medium, 3=high

private:
    bool initWithArea(const cocos2d::Size &area);
    void spawnDot();
    int getMaxDots() const;

    cocos2d::Size _area;
    bool _running = false;
    bool _paused = false;
    int _qualityLevel = 2;  // Default: medium

    // Configurable limits based on quality
    static constexpr int MAX_DOTS_HIGH = 25;
    static constexpr int MAX_DOTS_MEDIUM = 15;
    static constexpr int MAX_DOTS_LOW = 8;
};

#endif  // __AMBIENT_PARTICLES_H__
