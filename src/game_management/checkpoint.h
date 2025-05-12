#pragma once
class Checkpoint {
public:
    Checkpoint();

    int get_checkpoint_id() const;
    void set_checkpoint_id(int id);
    bool is_checkpoint_active() const;
    void activate_checkpoint();
    void deactivate_checkpoint();

protected:
    int checkpointId_;
    bool isCheckpointActive_;
};
