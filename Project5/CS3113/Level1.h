#ifndef LEVEL1_H
#define LEVEL1_H

#include "Scene.h"
#include "raylib.h"

class Level1 : public Scene {
public:
    Texture2D bg;

    void init() override;
    void processInput() override;
    void update(float dt) override;
    void render() override;
};

#endif
