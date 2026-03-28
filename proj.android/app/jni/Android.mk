LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/scene/GameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/scene/LevelMenuScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/GameModel.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/LevelData.cpp \
                   $(LOCAL_PATH)/../../../Classes/model/ScoreManager.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/AimLineView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/ArenaView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/BallView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/HUD.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/TrayView.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/VFXHelper.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/AmbientParticles.cpp \
                   $(LOCAL_PATH)/../../../Classes/view/ObstacleView.cpp \
                   $(LOCAL_PATH)/../../../Classes/controller/InputController.cpp \
                   $(LOCAL_PATH)/../../../Classes/controller/GameController.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../Classes/common \
                    $(LOCAL_PATH)/../../../Classes/scene \
                    $(LOCAL_PATH)/../../../Classes/model \
                    $(LOCAL_PATH)/../../../Classes/view \
                    $(LOCAL_PATH)/../../../Classes/controller

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cc_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
