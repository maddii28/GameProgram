#ifndef LEVEL2_H
#define LEVEL2_H

#include "Scene.h"
#include "raylib.h"
#include "Entity.h"
#include <vector>
#include <string>
#include <map>

struct Arrow {
    Entity e;
    Direction dir;
    int lane;
    bool active;
    bool aiScored;
};

struct LyricBlock {
    float time;
    std::vector<std::string> lines;
};

class Level2 : public Scene {
public:
    // --- GAME STATE ---
    Texture2D background;

    Texture2D texUp;
    Texture2D texDown;
    Texture2D texLeft;
    Texture2D texRight;

    Music music;
    Sound hit1, hit2, hit3, hit4;

    float songLength = 0;
    float songTime = 0;
    bool gameEnded = false;

    Shader loseShader;
    bool loseEffect = false;
    float loseTime = 0.0f;

    bool mPlayerLost = false;
    bool mLoseShaderActive = false;

    int score = 0;
    int aiScore = 0;

    Entity* player;
    Entity* ai;

    std::vector<Arrow> arrows;
    float arrowSpawnTimer = 0;

    std::vector<LyricBlock> lyrics;
    int currentLyricIndex = -1;

    float prevTicks = 0;
    float accumulator = 0;


    void init() override;
    void processInput() override;
    void update(float dt) override;
    void render() override;


    ~Level2();
};

#endif
