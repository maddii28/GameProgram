// CS3113/Scene.h
#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"   // includes Map.h and cs3113.h

// One GameState per scene
struct GameState
{
    Entity *AxeKing   = nullptr;
    Map    *map       = nullptr;
    Entity *Block     = nullptr;
    Entity *samurai   = nullptr;
    Entity *robber   = nullptr;
    Entity *flyer = nullptr;


    Camera2D camera{};

    // 0 = stay here, >0 = index in level list to switch to
    int nextSceneID = -1;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() = default;

    // All scenes must implement these:
    virtual void initialise()         = 0;
    virtual void update(float dt)     = 0;
    virtual void render()             = 0;
    virtual void shutdown()           = 0;

    // Accessors – same style as prompt:
    GameState   getState()           const { return mGameState;       }
    Vector2     getOrigin()          const { return mOrigin;          }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif // SCENE_H
