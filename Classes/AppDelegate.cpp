#include "AppDelegate.h"

#include "Version.h"
#include "scene/LevelMenuScene.h"

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

namespace
{

static constexpr const char *APP_TITLE = "my-tennis-balls (" VERSION_STRING ")";

/// Resolution tiers: design reference -> low-end -> full HD -> 2K
const cocos2d::Size designResolutionSize(960, 540);
const cocos2d::Size smallResolutionSize(480, 320);
const cocos2d::Size mediumResolutionSize(1920, 1080);
const cocos2d::Size largeResolutionSize(2560, 1440);

}  // namespace

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
    // -- Cleanup audio engine --
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// -- GL context attribute setup --

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

namespace
{
int register_all_packages()
{
    return 0;
}
}  // namespace

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(
            APP_TITLE, cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create(APP_TITLE);
#endif
        director->setOpenGLView(glview);
    }

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    director->setDisplayStats(true);
#else
    director->setDisplayStats(false);
#endif

    director->setAnimationInterval(1.0f / 60);

    // Vertical full display, horizontal auto-adapt to various aspect ratios
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
                                    ResolutionPolicy::FIXED_HEIGHT);
    auto frameSize = glview->getFrameSize();
    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height,
                                            largeResolutionSize.width / designResolutionSize.width));
    } else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height,
                                            mediumResolutionSize.width / designResolutionSize.width));
    } else {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height,
                                            smallResolutionSize.width / designResolutionSize.width));
    }

    // -- Random seed initialization --
    register_all_packages();

    CCLOG("Application version: %s", VERSION_STRING);
    CCLOG("Version components: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    // -- Switch to level selection scene --
    auto scene = LevelMenuScene::createScene();
    director->runWithScene(scene);

    return true;
}

// -- Foreground/background switching --

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
