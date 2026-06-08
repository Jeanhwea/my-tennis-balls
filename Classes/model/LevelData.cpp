#include "LevelData.h"

#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

namespace
{

/// 解析单个关卡的 JSON 字符串，成功返回 true，失败返回 false。
bool parseLevel(const std::string &json, LevelData &outLevel)
{
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    if (doc.HasParseError()) {
        CCLOG("[LevelData] ERROR: JSON parse error at offset %zu: %d", doc.GetErrorOffset(),
              static_cast<int>(doc.GetParseError()));
        return false;
    }

    if (!doc.HasMember("id") || !doc["id"].IsInt()) {
        CCLOG("[LevelData] ERROR: Missing or invalid 'id' field");
        return false;
    }
    if (!doc.HasMember("name") || !doc["name"].IsString()) {
        CCLOG("[LevelData] ERROR: Missing or invalid 'name' field");
        return false;
    }
    if (!doc.HasMember("maxBalls") || !doc["maxBalls"].IsInt()) {
        CCLOG("[LevelData] ERROR: Missing or invalid 'maxBalls' field");
        return false;
    }
    if (!doc.HasMember("trays") || !doc["trays"].IsArray()) {
        CCLOG("[LevelData] ERROR: Missing or invalid 'trays' array");
        return false;
    }

    LevelData level;
    level.id = doc["id"].GetInt();
    level.name = doc["name"].GetString();
    level.maxBalls = doc["maxBalls"].GetInt();

    const auto &trays = doc["trays"];
    for (rapidjson::SizeType i = 0; i < trays.Size(); ++i) {
        const auto &t = trays[i];

        if (!t.IsObject()) {
            CCLOG("[LevelData] ERROR: Tray %zu is not an object", static_cast<size_t>(i));
            return false;
        }
        if (!t.HasMember("x") || !t["x"].IsNumber()) {
            CCLOG("[LevelData] ERROR: Tray %zu missing or invalid 'x'", static_cast<size_t>(i));
            return false;
        }
        if (!t.HasMember("y") || !t["y"].IsNumber()) {
            CCLOG("[LevelData] ERROR: Tray %zu missing or invalid 'y'", static_cast<size_t>(i));
            return false;
        }
        if (!t.HasMember("width") || !t["width"].IsNumber()) {
            CCLOG("[LevelData] ERROR: Tray %zu missing or invalid 'width'", static_cast<size_t>(i));
            return false;
        }
        if (!t.HasMember("targets") || !t["targets"].IsInt()) {
            CCLOG("[LevelData] ERROR: Tray %zu missing or invalid 'targets'", static_cast<size_t>(i));
            return false;
        }

        TrayData tray;
        tray.x = static_cast<float>(t["x"].GetDouble());
        tray.y = static_cast<float>(t["y"].GetDouble());
        tray.width = static_cast<float>(t["width"].GetDouble());
        tray.targets = t["targets"].GetInt();
        level.trays.push_back(tray);
    }

    outLevel = level;
    return true;
}

std::vector<LevelData> loadAllLevels()
{
    std::vector<LevelData> levels;

    for (int i = 1;; ++i) {
        auto filename = StringUtils::format("levels/level_%03d.json", i);

        if (!FileUtils::getInstance()->isFileExist(filename)) {
            break;
        }

        auto content = FileUtils::getInstance()->getStringFromFile(filename);
        if (content.empty()) {
            CCLOG("[LevelData] WARNING: Empty content in %s, skipping", filename.c_str());
            continue;
        }

        LevelData level;
        if (parseLevel(content, level)) {
            levels.push_back(level);
            CCLOG("Loaded level %d: %s", levels.back().id, levels.back().name.c_str());
        } else {
            CCLOG("[LevelData] WARNING: Failed to parse %s, skipping", filename.c_str());
        }
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
