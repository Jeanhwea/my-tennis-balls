#include "GameModel.h"

bool GameModel::hasNextLevel() const
{
    return _levelIndex + 1 < static_cast<int>(getAllLevels().size());
}

const LevelData &GameModel::currentLevel() const
{
    return getAllLevels().at(_levelIndex);
}
