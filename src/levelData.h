#pragma once

#include <nlohmann/json.h>

#include <vector>

using json = nlohmann::json;

namespace synthrush {

struct LevelData {
    std::string levelName;
    std::string audioFile;
    std::vector<int> beats;

    LevelData(json j) {
        levelName = j.at("levelName").get<std::string>();
        audioFile = j.at("audioFile").get<std::string>();
        beats = j.at("beats").get<std::vector<int>>();
    }
};

}  // namespace synthrush
