#include "map_loader.h"

#include <fstream>
#include <stdexcept>
#include "../../objects/barrel/barrel.h"
#include "../../objects/decos/spotlight_point.h"
#include "../../objects/cars/game_car/game_car.h"
#include <iostream>
#include "../../../nlohmann/json.hpp"
#include "../../objects/floor/floor.h"


MapLoader::MapLoader(
    std::vector<std::shared_ptr<SceneObject>>& sceneVec,
    std::vector<std::shared_ptr<ParticleObject>>& partVec,
    std::vector<std::shared_ptr<CollisionObject>>& collVec,
    std::vector<std::shared_ptr<LightObject>>& lightVec,
    std::vector<std::shared_ptr<UploaderObject>>& uploadVec,
    CollisionManager& collMgr
)
    : sceneObjs_(sceneVec)
    , partObjs_(partVec)
    , collObjs_(collVec)
    , lightObjs_(lightVec)
    , uploadObjs_(uploadVec)
    , collMgr_(collMgr)
{
    registerType<Barrel>("Barrel");
    registerType<SpotlightPoint>("SpotlightPoint");
    registerType <Floor> ("Floor");

}

void MapLoader::loadFromFile(const std::string& filename) {
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

            for (const auto& typeNameJson : maps.at(code)) {
                auto typeName = typeNameJson.get<std::string>();
                auto it = factories_.find(typeName);
                if (it == factories_.end())
                    continue;

                auto so = it->second();

                float worldX = x * tileSize - halfWidth + tileSize * 0.5f;
                float worldZ = y * tileSize - halfHeight + tileSize * 0.5f;

                so->set_position({ worldX, 0.0f, worldZ });

                // every object is a SceneObject
                sceneObjs_.push_back(so);
                so->init();

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
            }
        }
    }
}
