#include "map_loader.h"

#include <fstream>
#include <stdexcept>
#include "../../objects/barrel/barrel.h"
#include "../../objects/decos/spotlight_point.h"
#include "../../objects/cars/game_car/game_car.h"
#include <iostream>
#include "../../../nlohmann/json.hpp"
#include "../../objects/floor/floor.h"
#include "../../objects/interactable/game_checkpoint.h"
#include "../../game_management/game_manager.h"


MapLoader::MapLoader(
    std::shared_ptr<GameCar>& gameCar,
    std::vector<std::shared_ptr<SceneObject>>& sceneVec,
    std::vector<std::shared_ptr<ParticleObject>>& partVec,
    std::vector<std::shared_ptr<CollisionObject>>& collVec,
    std::vector<std::shared_ptr<LightObject>>& lightVec,
    std::vector<std::shared_ptr<UploaderObject>>& uploadVec,
    std::vector<std::shared_ptr<Checkpoint>>& checkpointVec,
    CollisionManager& collMgr
)
    : gameCar_(gameCar)
    , sceneObjs_(sceneVec)
    , partObjs_(partVec)
    , collObjs_(collVec)
    , lightObjs_(lightVec)
    , uploadObjs_(uploadVec)
    , checkpointObjs_(checkpointVec)
    , collMgr_(collMgr)
{
    register_type<GameCar>("GameCar");

    register_type<Barrel>("Barrel");
    register_type<SpotlightPoint>("SpotlightPoint");
    register_type <Floor> ("Floor");
    register_type<GameCheckpoint>("Checkpoint");

}

void MapLoader::load_from_file(const std::string& filename) {
    const std::string fullPath = mapFolder_ + filename;
    nlohmann::json j;
    std::ifstream in{ fullPath };
    if (!in.is_open())
        std::cout << "ERROR: Couldnt open given map file " << fullPath << "\n";
    in >> j;

    int width = j.at("width").get<int>();
    int height = j.at("height").get<int>();
    auto grid = j.at("grid");
    auto maps = j.at("mappings");

    const float tileSize = 20.0f;
    float halfWidth = width * tileSize * 0.5f;
    float halfHeight = height * tileSize * 0.5f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto code = grid.at(y).at(x).get<std::string>();
            if (code == "__" || !maps.contains(code))
                continue;

            for (const auto& entry : maps.at(code)) {
                auto props = &entry;
                auto typeName = (*props).at("type").get<std::string>();

                auto it = factories_.find(typeName);
                if (it == factories_.end())
                    continue;


                auto so = it->second();

                float worldX = x * tileSize - halfWidth + tileSize * 0.5f;
                float worldZ = y * tileSize - halfHeight + tileSize * 0.5f;


                if (props->contains("rotation")) {
                    float r = props->at("rotation");
                    so->set_rotation(glm::radians(r));
                }

                so->set_position({ worldX, 0.0f, worldZ });

                // every object is a SceneObject
                sceneObjs_.push_back(so);
                so->init();

                if (auto gc = std::dynamic_pointer_cast<GameCar>(so))
                    gameCar_ = gc;

                // then try each interface
                if (auto po = std::dynamic_pointer_cast<ParticleObject>(so))
                    partObjs_.push_back(po);

                if (auto co = std::dynamic_pointer_cast<CollisionObject>(so)) {
                    collObjs_.push_back(co);
                    collMgr_.addObject(co);
                }

                if (auto lo = std::dynamic_pointer_cast<LightObject>(so)) {
                    if (auto sp = std::dynamic_pointer_cast<SpotlightPoint>(so)) {
                        sp->set_rotate_pos({ worldX, 0.0f, worldZ });
                    }
                    lightObjs_.push_back(lo);
                    lo->initShadowMap();
                }

                if (auto uo = std::dynamic_pointer_cast<UploaderObject>(so))
                    uploadObjs_.push_back(uo);

                if (auto co = std::dynamic_pointer_cast<Checkpoint>(so)) {
                    GameManager::instance().add_checkpoint(co);
                    checkpointObjs_.push_back(co);
                    if (props->contains("id")) {
                        auto id = props->at("id").get<int>();
                        co->set_checkpoint_id(id);
                    }
                }
            }
        }
    }
}
