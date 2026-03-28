#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__

#include <functional>

#include "cocos2d.h"

/// 处理触摸输入，将拖拽手势转换为发射命令。
class InputController
{
public:
    struct LaunchCommand {
        cocos2d::Vec2 position;
        cocos2d::Vec2 velocity;
    };

    using OnLaunchCallback = std::function<void(const LaunchCommand &)>;
    using OnDragCallback = std::function<void(const cocos2d::Vec2 &start, const cocos2d::Vec2 &delta)>;
    using OnDragEndCallback = std::function<void()>;

    void setOnLaunch(OnLaunchCallback cb) { _onLaunch = std::move(cb); }

    void setOnDrag(OnDragCallback cb) { _onDrag = std::move(cb); }

    void setOnDragEnd(OnDragEndCallback cb) { _onDragEnd = std::move(cb); }

    /// 设置有效发射起始位置的最小 X 坐标（右侧区域）。
    void setLaunchZoneMinX(float minX) { _launchZoneMinX = minX; }

    /// 在指定节点上注册触摸监听器。
    void init(cocos2d::Node *target);

private:
    bool _isDragging = false;
    cocos2d::Vec2 _dragStart;
    float _launchZoneMinX = 0.0f;

    OnLaunchCallback _onLaunch;
    OnDragCallback _onDrag;
    OnDragEndCallback _onDragEnd;

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif  // __INPUT_CONTROLLER_H__
