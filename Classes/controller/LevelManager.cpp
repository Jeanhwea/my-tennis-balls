#include "LevelManager.h"

#include "common/GameConstants.h"
#include "scene/LevelMenuScene.h"
#include "view/ArenaView.h"
#include "view/TrayView.h"

USING_NS_CC;

void LevelManager::init(Scene *scene, const Size &visibleSize, GameModel &model, BallManager &ballManager, int startLevel)
{
    _scene = scene;
    _visibleSize = visibleSize;
    _model = &model;
    _ballManager = &ballManager;
    _ballManager->clear();
    loadLevel(startLevel);
}

void LevelManager::update(float dt)
{
    if (!_transitioning) {
        _ballManager->collectOutOfBounds(_scene, _visibleSize);
        _ballManager->processPendingRemovals();
    }
}

bool LevelManager::loadLevel(int index)
{
    if (index < 0 || index >= static_cast<int>(getAllLevels().size())) {
        CCLOG("[LevelManager] ERROR: level index %d out of range!", index);
        return false;
    }
    _transitioning = false;
    _ballCounter = 0;

    _scene->stopAllActions();
    clearLevelNodes();

    _model->loadLevel(index);
    const auto &level = _model->currentLevel();

    cocos2d::Vector<Node *> targets;
    int totalTargets = TrayView::createFromLevel(_scene, _visibleSize, level, targets);
    _model->setTargetsRemaining(totalTargets);
    _ballManager->initFromLevel();

    if (_onLevelChanged) {
        _onLevelChanged(index);
    }
    return true;
}

void LevelManager::clearLevelNodes()
{
    _ballManager->clear();

    std::vector<Node *> toRemove;
    for (auto child : _scene->getChildren()) {
        int tag = child->getTag();
        if (tag == TAG_BALL || tag == TAG_TARGET || tag == TAG_TRAY) {
            toRemove.push_back(child);
        }
    }
    for (auto node : toRemove) node->removeFromParent();
}

void LevelManager::onLevelCleared()
{
    if (_transitioning) return;
    _transitioning = true;

    if (_model->hasNextLevel()) {
        int nextIdx = _model->levelIndex() + 1;
        _scene->runAction(Sequence::create(
            DelayTime::create(2.5f), CallFunc::create([this, nextIdx]() { loadLevel(nextIdx); }), nullptr));
    } else {
        _scene->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([this]() {
                                               LevelMenuScene::setInitialLevelIndex(_model->levelIndex());
                                               auto menuScene = LevelMenuScene::createScene();
                                               Director::getInstance()->replaceScene(TransitionFade::create(
                                                   0.5f, menuScene, Color3B(10, 10, 30)));
                                           }),
                                           nullptr));
    }
}

void LevelManager::onLevelFailed()
{
    if (_transitioning) return;
    _transitioning = true;

    int curIdx = _model->levelIndex();
    _scene->runAction(Sequence::create(DelayTime::create(2.5f),
                                       CallFunc::create([this, curIdx]() { loadLevel(curIdx); }), nullptr));
}

void LevelManager::checkFailCondition()
{
    if (_transitioning) return;
    if (_model->isCleared()) return;
    const auto &level = _model->currentLevel();
    if (_ballCounter >= level.maxBalls && _ballManager->ballCount() == 0) {
        onLevelFailed();
    }
}
