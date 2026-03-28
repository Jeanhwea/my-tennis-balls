#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"

/**
 * @brief    The cocos2d Application.
 *
 * Private inheritance here hides part of interface from Director.
 */
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
