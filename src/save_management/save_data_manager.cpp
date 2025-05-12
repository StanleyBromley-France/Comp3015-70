#include "save_data_manager.h"
#include <fstream>

using json = nlohmann::json;

static const char* ColourStringNames[] = {
    "Orange","Black","Blue","DarkBlue","DarkGrey","Grey","Red"
};

std::string to_string(CarColours::Index c) {
    return ColourStringNames[size_t(c)];
}

std::optional<CarColours::Index> from_string(std::string const& s) {
    for (size_t i = 0; i < std::size(ColourStringNames); ++i)
        if (s == ColourStringNames[i])
            return CarColours::Index(i);
    return std::nullopt;
}

// jsoon parsing
static void to_json(json& j, SaveData const& s) {
    j = json{
      {"carColour1", to_string(s.carColour1)},
      {"carColour2", to_string(s.carColour2)},
      {"bestTime", s.bestTime} 
    };
}

static void from_json(json const& j, SaveData& s) {
    // colours
    if (auto c1 = from_string(j.value("carColour1", ""))) s.carColour1 = *c1;
    if (auto c2 = from_string(j.value("carColour2", ""))) s.carColour2 = *c2;
    
    s.bestTime = j.value("bestTime", s.bestTime);
}

SaveDataManager& SaveDataManager::Instance() {
    static SaveDataManager inst;
    return inst;
}

SaveDataManager::SaveDataManager()
    : data_{} // initialised to defaults
    , save_path_("data/save_data.json")
{}

bool SaveDataManager::Load() {
    try {
        if (!std::filesystem::exists(save_path_))
            return false; 

        std::ifstream file(save_path_);
        if (!file.is_open())
            return false;

        json j;
        file >> j;
        data_ = j.get<SaveData>(); // gets data from save file
        return true;
    }
    catch (...) {
        return false;
    }
}

bool SaveDataManager::Save() const {
    try {
        std::filesystem::create_directories(save_path_.parent_path());
        json j = data_;
        std::ofstream file(save_path_);
        if (!file.is_open()) return false;
        file << j.dump(4);
        return true;
    }
    catch (...) {
        return false;
    }
}
