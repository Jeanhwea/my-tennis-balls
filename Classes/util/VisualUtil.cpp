#include "VisualUtil.h"

#include "AmbientParticles.h"

USING_NS_CC;

void VisualUtil::drawGradientBackground(Node *parent, const Size &size,
                                        const GradientConfig &config, int zOrder)
{
    auto bg = DrawNode::create();
    for (int i = 0; i < config.strips; ++i) {
        float t0 = static_cast<float>(i) / config.strips;
        float t1 = static_cast<float>(i + 1) / config.strips;
        Color4F c(config.botColor.r + (config.topColor.r - config.botColor.r) * (t0 + t1) / 2,
                   config.botColor.g + (config.topColor.g - config.botColor.g) * (t0 + t1) / 2,
                   config.botColor.b + (config.topColor.b - config.botColor.b) * (t0 + t1) / 2,
                   1.0f);
        bg->drawSolidRect(Vec2(0, size.height * t0), Vec2(size.width, size.height * t1), c);
    }
    parent->addChild(bg, zOrder);
}

void VisualUtil::drawGrid(Node *parent, const Size &size,
                          const GridConfig &config, int zOrder)
{
    auto grid = DrawNode::create();
    float maxW = (config.maxWidth > 0.0f) ? config.maxWidth : size.width;
    for (float x = 0; x <= maxW; x += config.spacing) {
        grid->drawLine(Vec2(x, 0), Vec2(x, size.height), config.color);
    }
    for (float y = 0; y <= size.height; y += config.spacing) {
        grid->drawLine(Vec2(0, y), Vec2(maxW, y), config.color);
    }
    parent->addChild(grid, zOrder);
}

void VisualUtil::drawCorners(Node *parent, const Size &size,
                             const CornerConfig &config, int zOrder)
{
    auto corners = DrawNode::create();
    float w = size.width;
    float h = size.height;
    float rEdge = (config.rightEdge > 0.0f) ? config.rightEdge : w;

    // Top-left
    corners->drawSolidRect(Vec2(config.pad, h - config.pad - config.thick),
                           Vec2(config.pad + config.len, h - config.pad), config.color);
    corners->drawSolidRect(Vec2(config.pad, h - config.pad - config.len),
                           Vec2(config.pad + config.thick, h - config.pad), config.color);
    // Top-right
    corners->drawSolidRect(Vec2(rEdge - config.pad - config.len, h - config.pad - config.thick),
                           Vec2(rEdge - config.pad, h - config.pad), config.color);
    corners->drawSolidRect(Vec2(rEdge - config.pad - config.thick, h - config.pad - config.len),
                           Vec2(rEdge - config.pad, h - config.pad), config.color);
    // Bottom-left
    corners->drawSolidRect(Vec2(config.pad, config.pad),
                           Vec2(config.pad + config.len, config.pad + config.thick), config.color);
    corners->drawSolidRect(Vec2(config.pad, config.pad),
                           Vec2(config.pad + config.thick, config.pad + config.len), config.color);
    // Bottom-right
    corners->drawSolidRect(Vec2(rEdge - config.pad - config.len, config.pad),
                           Vec2(rEdge - config.pad, config.pad + config.thick), config.color);
    corners->drawSolidRect(Vec2(rEdge - config.pad - config.thick, config.pad),
                           Vec2(rEdge - config.pad, config.pad + config.len), config.color);

    parent->addChild(corners, zOrder);
}

void VisualUtil::addAmbientParticles(Node *parent, const Size &area, int zOrder)
{
    auto ambient = AmbientParticles::create(area);
    parent->addChild(ambient, zOrder);
    ambient->start();
}
