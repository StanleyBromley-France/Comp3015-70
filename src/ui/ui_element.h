#pragma once

class UIElement {
public:
    virtual ~UIElement() = default;

    // Called once when the ui is first loaded
    virtual void init() = 0;

    // Called every frame
    virtual void update() = 0;

    // Called every frame after update
    virtual void render() = 0;

};