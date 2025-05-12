#include "checkpoint.h"
Checkpoint::Checkpoint() : checkpointId_(-1), isCheckpointActive_(false) {}

int Checkpoint::get_checkpoint_id() const {
    return checkpointId_;
}

void Checkpoint::set_checkpoint_id(int id)
{
    checkpointId_ = id;
}

bool Checkpoint::is_checkpoint_active() const {
    return isCheckpointActive_;
}

void Checkpoint::activate_checkpoint() {
    isCheckpointActive_ = true;
}

void Checkpoint::deactivate_checkpoint() {
    isCheckpointActive_ = false;
}
