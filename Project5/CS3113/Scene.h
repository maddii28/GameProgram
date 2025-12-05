#ifndef SCENE_H
#define SCENE_H

#include "raylib.h"

class Scene {
public:
    virtual ~Scene() {}
    virtual void init() = 0;
    virtual void processInput() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;

    bool finished = false;  
    int nextScene = -1;     
};

#endif