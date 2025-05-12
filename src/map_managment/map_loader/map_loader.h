// MapLoader.hpp
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

class MapLoader {
public:
    MapLoader(
        std::vector<std::shared_ptr<SceneObject>>& sceneVec,
        std::vector<std::shared_ptr<ParticleObject>>& partVec,
        std::vector<std::shared_ptr<CollisionObject>>& collVec,
        std::vector<std::shared_ptr<LightObject>>& lightVec,
        std::vector<std::shared_ptr<UploaderObject>>& uploadVec,
        CollisionManager& collMgr
    );

    template<typename T>
    void registerType(const std::string& typeName) {
        factories_[typeName] = []() {
            return std::static_pointer_cast<SceneObject>(std::make_shared<T>());
            };
    }

    void loadFromFile(const std::string& filename);

private:
    std::string mapFolder_ = "maps/";

    std::unordered_map<std::string, std::function<std::shared_ptr<SceneObject>()>> factories_;

    std::vector<std::shared_ptr<SceneObject>>& sceneObjs_;
    std::vector<std::shared_ptr<ParticleObject>>& partObjs_;
    std::vector<std::shared_ptr<CollisionObject>>& collObjs_;
    std::vector<std::shared_ptr<LightObject>>& lightObjs_;
    std::vector<std::shared_ptr<UploaderObject>>& uploadObjs_;
    CollisionManager& collMgr_;
};