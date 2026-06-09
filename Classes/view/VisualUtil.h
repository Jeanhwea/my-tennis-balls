/// @file VisualUtil.h
/// Shared visual drawing utilities for background gradients, grids, corner decorations, and ambient particles.
/// Extracted from ArenaView and LevelMenuScene to eliminate code duplication.

#ifndef __VISUAL_UTIL_H__
#define __VISUAL_UTIL_H__

#include "cocos2d.h"

/// @namespace VisualUtil
/// Utility functions for drawing common visual elements like gradient backgrounds, grid lines,
/// corner decorations, and creating ambient particle systems.
namespace VisualUtil
{

struct GradientConfig
{
    cocos2d::Color4F botColor;
    cocos2d::Color4F topColor;
    int strips;
};

struct GridConfig
{
    float spacing;
    cocos2d::Color4F color;
    float maxWidth;  ///< 0 means full width (same as visibleSize.width)
};

struct CornerConfig
{
    float len;
    float thick;
    float pad;
    cocos2d::Color4F color;
    float rightEdge;  ///< The right edge X coordinate (e.g., launch zone edge or width)
};

/// Draw a vertical gradient background using horizontal strips.
/// @param parent The parent node to add the background to.
/// @param size The visible size.
/// @param config Gradient configuration.
/// @param zOrder Z order for the background node.
void drawGradientBackground(cocos2d::Node *parent, const cocos2d::Size &size,
                            const GradientConfig &config, int zOrder = -10);

/// Draw a rectangular grid of lines.
/// @param parent The parent node to add the grid to.
/// @param size The visible size.
/// @param config Grid configuration.
/// @param zOrder Z order for the grid node.
void drawGrid(cocos2d::Node *parent, const cocos2d::Size &size,
              const GridConfig &config, int zOrder = -9);

/// Draw 8 corner decorations (4 corners, each with two perpendicular bars).
/// @param parent The parent node to add the corner decorations to.
/// @param size The visible size.
/// @param config Corner configuration.
/// @param zOrder Z order for the corners node.
void drawCorners(cocos2d::Node *parent, const cocos2d::Size &size,
                 const CornerConfig &config, int zOrder = -7);

/// Create and add ambient floating particles to a parent node.
/// @param parent The parent node to add the ambient particles to.
/// @param area The area (size) for the particles.
/// @param zOrder Z order for the ambient particles node.
void addAmbientParticles(cocos2d::Node *parent, const cocos2d::Size &area, int zOrder = -5);

}  // namespace VisualUtil

#endif  // __VISUAL_UTIL_H__
