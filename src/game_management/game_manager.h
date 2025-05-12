#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include "checkpoint.h"

class GameManager {
public:
    static GameManager& instance() {
        static GameManager _instance;
        return _instance;
    }

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    GameManager(GameManager&&) = delete;
    GameManager& operator=(GameManager&&) = delete;

    void add_checkpoint(std::shared_ptr<Checkpoint> checkpoint);

    void start_race();

    void arrive_at_checkpoint(int checkpointId);

    float get_elapsed_time() const;
    bool has_race_ended();
    void reset();

private:
    GameManager() = default;
    ~GameManager() = default;

    std::vector<std::shared_ptr<Checkpoint>> checkpoints_;
    size_t currentCheckpointIndex_ = 0;
    std::chrono::steady_clock::time_point raceStartTime_;
    bool hasRaceStarted_ = false;
    bool hasRaceEnded = false;
};
