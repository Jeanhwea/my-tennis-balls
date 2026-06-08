/// @file AppDelegate.h
/// cocos2d-x application entry point, responsible for app lifecycle management and scene initialization.

#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"

/// @class AppDelegate
/// cocos2d application entry point, privately inherits to hide some interfaces from Director.
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
