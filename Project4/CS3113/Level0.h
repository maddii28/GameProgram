#ifndef LEVEL0_H
#define LEVEL0_H

#include "Scene.h"

class Level0 : public Scene
{
public:
    Level0();
    Level0(Vector2 origin, const char *bgHexCode);
    ~Level0() override = default;

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL0_H
