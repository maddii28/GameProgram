#ifndef LEVEL3_H
#define LEVEL3_H

#include "Scene.h"

class Level3 : public Scene
{
private:
    static constexpr int LEVEL_WIDTH  = 14;
    static constexpr int LEVEL_HEIGHT = 8;

    // You can tweak this, but I'll reuse a layout similar to Level2
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 4,
        4, 2, 2, 2, 0, 0, 0, 2, 2, 2, 3, 3, 3, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

public:
    static constexpr float TILE_DIMENSION          = 68.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 800.0f;

    Level3();
    Level3(Vector2 origin, const char *bgHexCode);
    ~Level3() override = default;

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL3_H
