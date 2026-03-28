#include "GameModel.h"

#include "cocos2d.h"

USING_NS_CC;

bool GameModel::hasNextLevel() const
{
    return _levelIndex + 1 < static_cast<int>(getAllLevels().size());
}

const LevelData &GameModel::currentLevel() const
{
    const auto &all = getAllLevels();
    CCLOG("[GameModel] currentLevel: index=%d, total=%d", _levelIndex, static_cast<int>(all.size()));
    CC_ASSERT(_levelIndex >= 0 && _levelIndex < static_cast<int>(all.size()));
    return all.at(_levelIndex);
}
