#pragma once

#include <filesystem>
#include "../../nlohmann/json.hpp"
#include "../objects/cars/car_colours.h"

struct SaveData {
    CarColours::Index carColour1 = CarColours::Orange;
    CarColours::Index carColour2 = CarColours::Orange;
    float bestTime = 999.0f;
};

class SaveDataManager {
public:
    static SaveDataManager& Instance();

    // loads save data from file.  if file missing or invalid, data is stored at its defaults
    bool Load();

    // saves the current save data to file
    bool Save() const;

    // access the in-memory settings.  Modify fields here, then call `Save()`.
    SaveData& Data() { return data_; }

private:
    SaveDataManager();
    ~SaveDataManager() = default;

    // non-copyable
    SaveDataManager(SaveDataManager const&) = delete;
    SaveDataManager& operator=(SaveDataManager const&) = delete;

    SaveData  data_;
    const std::filesystem::path save_path_;
};
