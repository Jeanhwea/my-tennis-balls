#ifndef VISUAL_UTIL_H
#define VISUAL_UTIL_H

#include "cocos2d.h"

namespace VisualUtil
{

struct GradientConfig {
    cocos2d::Color4F botColor;
    cocos2d::Color4F topColor;
    int strips;
};

struct GridConfig {
    float spacing;
    cocos2d::Color4F color;
    float maxWidth;
};

struct CornerConfig {
    float len;
    float thick;
    float pad;
    cocos2d::Color4F color;
    float rightEdge;
};

void drawGradientBackground(cocos2d::Node *parent, const cocos2d::Size &size, const GradientConfig &config,
                            int zOrder = -10);

void drawGrid(cocos2d::Node *parent, const cocos2d::Size &size, const GridConfig &config, int zOrder = -9);

void drawCorners(cocos2d::Node *parent, const cocos2d::Size &size, const CornerConfig &config,
                 int zOrder = -7);

void addAmbientParticles(cocos2d::Node *parent, const cocos2d::Size &area, int zOrder = -5);

}  // namespace VisualUtil

#endif
