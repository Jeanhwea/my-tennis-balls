/// @file AppDelegate.h
/// cocos2d-x 应用程序入口，负责应用生命周期管理和场景初始化。

#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"

/// @class AppDelegate
/// cocos2d 应用程序入口，私有继承以对 Director 隐藏部分接口。
class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    ~AppDelegate() override;

    void initGLContextAttrs() override;
    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
};

#endif  // _APP_DELEGATE_H_
