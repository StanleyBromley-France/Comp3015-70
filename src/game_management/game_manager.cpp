#include "game_manager.h"
#include <algorithm>
#include <iostream>

void GameManager::add_checkpoint(std::shared_ptr<Checkpoint> checkpoint) {
    checkpoints_.push_back(checkpoint);
}

void GameManager::start_race() {
    if (checkpoints_.empty() || hasRaceStarted_) return;

    // sorts by ascending id, making sure they are ordered properly
    std::sort(checkpoints_.begin(), checkpoints_.end(),
        [](auto const& a, auto const& b) {
            return a->get_checkpoint_id() < b->get_checkpoint_id();
        });

    hasRaceStarted_ = true;
    currentCheckpointIndex_ = 0;
    checkpoints_[currentCheckpointIndex_]->activate_checkpoint();
    raceStartTime_ = std::chrono::steady_clock::now();

    std::cout << "Race started! Checkpoint "
        << checkpoints_[currentCheckpointIndex_]->get_checkpoint_id()
        << " activated.\n";
}

void GameManager::arrive_at_checkpoint(int checkpointId) {
    if (!hasRaceStarted_ || currentCheckpointIndex_ >= checkpoints_.size())
        return;

    auto& current = checkpoints_[currentCheckpointIndex_];
    if (current->get_checkpoint_id() == checkpointId) {
        current->deactivate_checkpoint();
        std::cout << "Checkpoint " << checkpointId << " reached.\n";

        ++currentCheckpointIndex_;
        if (currentCheckpointIndex_ < checkpoints_.size()) {
            checkpoints_[currentCheckpointIndex_]->activate_checkpoint();
            std::cout << "Next checkpoint "
                << checkpoints_[currentCheckpointIndex_]->get_checkpoint_id()
                << " activated.\n";
        }
        else {
            std::cout << "Race complete!\n";
            std::cout << get_elapsed_time() << "\n";
        }
    }
}

float GameManager::get_elapsed_time() const {
    if (!hasRaceStarted_) return 0.0f;
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - raceStartTime_;
    return elapsed.count();
}

void GameManager::reset() {
    // Deactivate any active checkpoint
    if (hasRaceStarted_ && currentCheckpointIndex_ < checkpoints_.size())
        checkpoints_[currentCheckpointIndex_]->deactivate_checkpoint();

    checkpoints_.clear();
    currentCheckpointIndex_ = 0;
    hasRaceStarted_ = false;
    // raceStartTime is irrelevant when raceStarted==false
    std::cout << "GameManager has been reset.\n";
}
