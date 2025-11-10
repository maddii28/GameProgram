// CS3113/Level2.h
#ifndef LEVEL2_H
#define LEVEL2_H

#include "Scene.h"

class Level2 : public Scene
{
private:
    static constexpr int LEVEL_WIDTH  = 14;
    static constexpr int LEVEL_HEIGHT = 8;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 6, 10, 22, 0, 0, 0, 25, 25, 24, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0
    };

public:
    static constexpr float TILE_DIMENSION          = 68.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 800.0f;

    Level2();
    Level2(Vector2 origin, const char *bgHexCode);
    ~Level2() override = default;

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL2_H
