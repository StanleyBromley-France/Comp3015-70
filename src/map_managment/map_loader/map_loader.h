#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include "../../objects/scene_object.h"
#include "../../objects/particle_object.h"
#include "../../collision/collision_object/collision_object.h"
#include "../../objects/light_object.h"
#include "../../objects/uploader_object.h"
#include "../../collision/collision_manager/collision_manager.h"
#include "../../game_management/checkpoint.h"
#include "../../objects/cars/game_car/game_car.h"

class MapLoader {
public:
    MapLoader(
        std::shared_ptr<GameCar>& gameCar,
        std::vector<std::shared_ptr<SceneObject>>& sceneVec,
        std::vector<std::shared_ptr<ParticleObject>>& partVec,
        std::vector<std::shared_ptr<CollisionObject>>& collVec,
        std::vector<std::shared_ptr<LightObject>>& lightVec,
        std::vector<std::shared_ptr<UploaderObject>>& uploadVec,
        std::vector<std::shared_ptr<Checkpoint>>& checkpointVec,
        CollisionManager& collMgr
    );

    template<typename T>
    void register_type(const std::string& typeName) {
        factories_[typeName] = []() {
            return std::static_pointer_cast<SceneObject>(std::make_shared<T>());
            };
    }

    void load_from_file(const std::string& filename);

private:
    std::string mapFolder_ = "maps/";

    std::unordered_map<std::string, std::function<std::shared_ptr<SceneObject>()>> factories_;
    
    std::shared_ptr<GameCar>& gameCar_;
    std::vector<std::shared_ptr<SceneObject>>& sceneObjs_;
    std::vector<std::shared_ptr<ParticleObject>>& partObjs_;
    std::vector<std::shared_ptr<CollisionObject>>& collObjs_;
    std::vector<std::shared_ptr<LightObject>>& lightObjs_;
    std::vector<std::shared_ptr<UploaderObject>>& uploadObjs_;
    std::vector<std::shared_ptr<Checkpoint>>& checkpointObjs_;

    CollisionManager& collMgr_;
};