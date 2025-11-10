#ifndef LEVEL1_H
#define LEVEL1_H

#include "Scene.h"

class Level1 : public Scene
{
private:
    static constexpr int LEVEL_WIDTH  = 14;
    static constexpr int LEVEL_HEIGHT = 8;

    
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 6, 25, 25, 25, 25, 24, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 2, 2, 2, 0, 0, 0, 2, 2, 2, 3, 3, 3, 4,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

public:
    static constexpr float TILE_DIMENSION          = 68.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 800.0f;

    Level1();
    Level1(Vector2 origin, const char *bgHexCode);
    ~Level1() override = default;

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL1_H
