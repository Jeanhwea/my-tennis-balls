
#ifndef AMBIENT_PARTICLES_H
#define AMBIENT_PARTICLES_H

#include "cocos2d.h"

class AmbientParticles : public cocos2d::Node
{
public:
    static AmbientParticles *create(const cocos2d::Size &area);

    void start();
    void stop();
    void pause();
    void resume();

    void setQualityLevel(int level);

private:
    bool initWithArea(const cocos2d::Size &area);
    void spawnDot();
    int getMaxDots() const;

    cocos2d::Size _area;
    bool _running = false;
    bool _paused = false;
    int _qualityLevel = 2;

    static constexpr int MAX_DOTS_HIGH = 25;
    static constexpr int MAX_DOTS_MEDIUM = 15;
    static constexpr int MAX_DOTS_LOW = 8;
};

#endif
