#pragma once
#include "../collision_object/collision_object.h"
#include <vector>
#include <memory>
#include <set>

class CollisionManager {
public:
    void addObject(std::shared_ptr<CollisionObject> obj);
    void removeObject(int id);
    void detectAndNotify();
    void detectOnceAndNotify();
    void clear();

private:
    std::vector<std::shared_ptr<CollisionObject>> objects_;
    int nextId_ { 0 };
    std::set<std::pair<int, int>> activeCollisions_;
};