#include "LevelData.h"

#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

namespace
{

LevelData parseLevel(const std::string &json)
{
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    LevelData level;
    level.id = doc["id"].GetInt();
    level.name = doc["name"].GetString();
    level.maxBalls = doc["maxBalls"].GetInt();

    const auto &trays = doc["trays"];
    for (rapidjson::SizeType i = 0; i < trays.Size(); ++i) {
        const auto &t = trays[i];
        TrayData tray;
        tray.x = static_cast<float>(t["x"].GetDouble());
        tray.y = static_cast<float>(t["y"].GetDouble());
        tray.width = static_cast<float>(t["width"].GetDouble());
        tray.targets = t["targets"].GetInt();
        level.trays.push_back(tray);
    }
    return level;
}

std::vector<LevelData> loadAllLevels()
{
    std::vector<LevelData> levels;

    for (int i = 1;; ++i) {
        auto filename = StringUtils::format("levels/level_%03d.json", i);
        if (!FileUtils::getInstance()->isFileExist(filename)) break;

        auto content = FileUtils::getInstance()->getStringFromFile(filename);
        if (content.empty()) break;

        levels.push_back(parseLevel(content));
        CCLOG("Loaded level %d: %s", levels.back().id, levels.back().name.c_str());
    }

    CCLOG("Total levels loaded: %d", static_cast<int>(levels.size()));
    return levels;
}

}  // namespace

const std::vector<LevelData> &getAllLevels()
{
    static auto levels = loadAllLevels();
    return levels;
}
