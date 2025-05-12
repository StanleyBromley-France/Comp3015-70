#include "collision_manager.h"
#include <algorithm>


void CollisionManager::addObject(std::shared_ptr<CollisionObject> obj) {
    obj->colliderId_ = nextId_++;
    objects_.push_back(obj);
}

void CollisionManager::removeObject(int id) {
    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [id](std::shared_ptr<CollisionObject> obj) { return obj->id() == id; }),
        objects_.end()
    );
}

void CollisionManager::detectAndNotify() {
    const int n = objects_.size();
    for (int i = 0; i < n; ++i) {
        auto& a = objects_[i];
        if (!a->collider_active()) continue;

        for (size_t j = i + 1; j < n; ++j) {
            auto& b = objects_[j];
            if (!b->collider_active()) continue;
            if (a->isStatic && b->isStatic) continue;

            auto collision = a->intersects(*b);
            if (collision.hasCollided) {
                a->on_collision(*b);
                b->on_collision(*a);

                a->set_collision_data(collision);

                // inverts normal and sets data for obj b
                collision.normal = -collision.normal;
                b->set_collision_data(collision);
            }
        }
    }
}

void CollisionManager::detectOnceAndNotify() {
    const int n = objects_.size();
    // creates a new set of collisions this frame to cleanup exited pairs later
    std::set<std::pair<int, int>> currentFrame;

    for (int i = 0; i < n; ++i) {
        auto& a = objects_[i];
        if (!a->collider_active()) continue;

        for (int j = i + 1; j < n; ++j) {
            auto& b = objects_[j];
            if (!b->collider_active()) continue;
            if (a->isStatic && b->isStatic) continue;
            auto collision = a->intersects(*b);
            if (collision.hasCollided) {
                // creates an id pair (smallID, largeID)
                int id1 = a->id(), id2 = b->id();
                auto pairId = std::minmax(id1, id2);

                // records that they're colliding this frame
                currentFrame.insert(pairId);

                // if this is the first frame of collision, fire callbacks
                // returns false here if it already exisits
                if (activeCollisions_.insert(pairId).second) {
                    a->on_collision_once(*b);
                    b->on_collision_once(*a);

                    a->set_collision_data(collision);

                    // inverts normal and sets data for obj b
                    collision.normal = -collision.normal;
                    b->set_collision_data(collision);
                }
            }
        }
    }

    // goes through active collision pairs and finds if any pairs match what is in currentFrame collision pairs
    // if not found then deleted as its no longer colliding
    for (auto it = activeCollisions_.begin(); it != activeCollisions_.end(); ) {
        if (currentFrame.count(*it) == 0)
            it = activeCollisions_.erase(it);
        else
            ++it;
    }
}


void CollisionManager::clear() {
    objects_.clear();
    nextId_ = 1;
}
