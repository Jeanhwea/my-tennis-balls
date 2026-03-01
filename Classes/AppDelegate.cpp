#include "AppDelegate.h"

#include "HelloWorldScene.h"
#include "Version.h"
#include "platform/desktop/CCGLViewImpl-desktop.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

#define APP_TITLE "my-tennis-balls (" VERSION_STRING ")"

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0;  // flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(
            APP_TITLE, cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height), 1.0f,
            true);
#else
        glview = GLViewImpl::create(APP_TITLE);
#endif
        director->setOpenGLView(glview);
    }

#if IS_DEBUG
    // turn on display FPS
    director->setDisplayStats(true);
#else
    // turn off display FPS
    director->setDisplayStats(false);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
                                    ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height,
                                            largeResolutionSize.width / designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height,
                                            mediumResolutionSize.width / designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height,
                                            smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();

    // Log application version
    CCLOG("Application version: %s", VERSION_STRING);
    CCLOG("Version components: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    // Add window resize listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [glview](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_F11) {
            auto glviewImpl = dynamic_cast<GLViewImpl*>(glview);
            if (glviewImpl) {
                if (glviewImpl->isFullscreen()) {
                    glviewImpl->setWindowed(designResolutionSize.width, designResolutionSize.height);
                } else {
                    glviewImpl->setFullscreen();
                }
            }
        }
    };
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, scene);

    // Add window resize event
    auto resizeListener =
        EventListenerCustom::create(GLViewImpl::EVENT_WINDOW_RESIZED, [](EventCustom* event) {
            auto director = Director::getInstance();
            auto glview = director->getOpenGLView();
            auto frameSize = glview->getFrameSize();

            // Reapply design resolution
            glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
                                            ResolutionPolicy::NO_BORDER);
            if (frameSize.height > mediumResolutionSize.height) {
                director->setContentScaleFactor(
                    MIN(largeResolutionSize.height / designResolutionSize.height,
                        largeResolutionSize.width / designResolutionSize.width));
            } else if (frameSize.height > smallResolutionSize.height) {
                director->setContentScaleFactor(
                    MIN(mediumResolutionSize.height / designResolutionSize.height,
                        mediumResolutionSize.width / designResolutionSize.width));
            } else {
                director->setContentScaleFactor(
                    MIN(smallResolutionSize.height / designResolutionSize.height,
                        smallResolutionSize.width / designResolutionSize.width));
            }
        });
    director->getEventDispatcher()->addEventListenerWithFixedPriority(resizeListener, 1);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
