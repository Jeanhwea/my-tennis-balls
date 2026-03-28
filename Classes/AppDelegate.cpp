#include "AppDelegate.h"

#include "Version.h"
#include "scene/LevelMenuScene.h"

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

namespace
{
const cocos2d::Size designResolutionSize(960, 540);    // 16:9 HD 基准
const cocos2d::Size smallResolutionSize(960, 540);     // qHD (mdpi/hdpi)
const cocos2d::Size mediumResolutionSize(1920, 1080);  // FHD (xhdpi)
const cocos2d::Size largeResolutionSize(2560, 1440);   // QHD (xxhdpi/xxxhdpi)
}  // namespace

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// 如需不同的上下文，请修改 glContextAttrs 的值
// 这会影响所有平台
void AppDelegate::initGLContextAttrs()
{
    // 设置 OpenGL 上下文属性：红、绿、蓝、透明度、深度、模板、多重采样数
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// 如需使用包管理器安装更多包，
// 请勿修改或删除此函数
static int register_all_packages()
{
    return 0;  // 包管理器标志
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化导演
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

#if IS_DEBUG
    // 开启 FPS 显示
    director->setDisplayStats(true);
#else
    // 关闭 FPS 显示
    director->setDisplayStats(false);
#endif

    // 设置帧率，不调用此方法时默认值为 1.0/60
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率
    // FIXED_HEIGHT 保证纵向完整显示，横向自适应不同宽高比（16:9、18:9、21:9 等）
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
                                    ResolutionPolicy::FIXED_HEIGHT);
    auto frameSize = glview->getFrameSize();
    // 如果帧高度大于中等尺寸的高度
    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height,
                                            largeResolutionSize.width / designResolutionSize.width));
    }
    // 如果帧高度大于小尺寸的高度
    else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height,
                                            mediumResolutionSize.width / designResolutionSize.width));
    }
    // 如果帧高度小于中等尺寸的高度
    else {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height,
                                            smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();

    // 记录应用版本
    CCLOG("Application version: %s", VERSION_STRING);
    CCLOG("Version components: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    // 创建关卡菜单场景
    auto scene = LevelMenuScene::createScene();

    // 运行
    director->runWithScene(scene);

    return true;
}

// 当应用进入非活跃状态时调用此函数。注意，接听电话时也会触发。
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

// 当应用重新变为活跃状态时调用此函数
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
