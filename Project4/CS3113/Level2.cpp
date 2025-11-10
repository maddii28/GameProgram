#include "Level2.h"
#include "cs3113.h"

extern AppStatus gAppStatus;
extern int  gLives;
extern bool gShowWin;
extern bool gShowLose;

constexpr Vector2 ATLAS_DIMENSIONS = { 1.0f, 6.0f };
constexpr Vector2 GBALTLAS_DIMENSIONS = { 1.0f, 6.0f };

Level2::Level2()
    : Scene()
{
}

Level2::Level2(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{}

void Level2::initialise()
{
    mGameState.nextSceneID = -1; 

    float sizeRatio = 48.0f / 64.0f;

    // Map
    mGameState.map = new Map(
        LEVEL_WIDTH,
        LEVEL_HEIGHT,
        mLevelData,
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/marble_packed.png",
        TILE_DIMENSION,
        9, 9,
        mOrigin
    );

    std::map<Direction, std::vector<int>> RobberAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6 } },
    };

    mGameState.robber= new Entity(
        { mOrigin.x, mOrigin.y - 230 },
        { 100.0f * sizeRatio, 80.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/GraveRobber_run.png",
        ATLAS,
        GBALTLAS_DIMENSIONS,
        RobberAnimationAtlas,
        NPC
    );

    mGameState.robber->setAIType(WANDERER);
    mGameState.robber->setAIState(WALKING);
    mGameState.robber->setColliderDimensions({
        mGameState.robber->getScale().x / 3.0f,
        8* mGameState.robber->getScale().y / 10.0f
    });
    mGameState.robber->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.robber->setDirection(LEFT);
    mGameState.robber->setSpeed(7*Entity::DEFAULT_SPEED/10);

    // Player
    std::map<Direction, std::vector<int>> AxeKingAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
    };

    mGameState.AxeKing = new Entity(
        { mOrigin.x - 250.0f, mOrigin.y - 250.0f },
        { 100.0f * sizeRatio, 95.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/Woodcutter_walk.png",
        ATLAS,
        ATLAS_DIMENSIONS,
        AxeKingAnimationAtlas,
        PLAYER
    );

    mGameState.AxeKing->setJumpingPower(550.0f);
    mGameState.AxeKing->setColliderDimensions({
        mGameState.AxeKing->getScale().x / 3.0f,
        6.0f * mGameState.AxeKing->getScale().y / 10.0f
    });
    mGameState.AxeKing->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    
    mGameState.Block = new Entity(
        { mOrigin.x - 90 , mOrigin.y + 190 },
        { 94.0f * sizeRatio, 70.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/portal.png",
        BLOCK
    );

    mGameState.Block->setColliderDimensions({
        mGameState.Block->getScale().x / 3.0f,
        mGameState.Block->getScale().y / 3.0f
    });

    // Camera
    mGameState.camera = {};
    mGameState.camera.target   = mGameState.AxeKing->getPosition();
    mGameState.camera.offset   = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;
}

void Level2::update(float deltaTime)
{


    mGameState.AxeKing->update(
        deltaTime,
        nullptr,
        mGameState.map,
        nullptr,
        0
    );

    mGameState.robber->update(
        deltaTime,
        mGameState.AxeKing,              
        mGameState.map,
        nullptr,
        0
    );

 
    if (mGameState.AxeKing->isColliding(mGameState.Block))
    {
        mGameState.nextSceneID = 3;
    }

    
if (mGameState.AxeKing->isColliding(mGameState.robber))
{
    gLives--;

    if (gLives > 0)
    {
        mGameState.nextSceneID = 2;  
    }
    else
    {
        gShowLose = true;
        mGameState.nextSceneID = 0;  
    }
}


if (mGameState.AxeKing->getPosition().y > END_GAME_THRESHOLD)
{
    gLives--;

    if (gLives > 0)
    {
        mGameState.nextSceneID = 2;  
    }
    else
    {
        gShowLose = true;
        mGameState.nextSceneID = 0;
    }
}


    mGameState.camera.target = mGameState.AxeKing->getPosition();
}

void Level2::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.AxeKing->render();
    mGameState.Block->render();
    mGameState.robber->render();
}

void Level2::shutdown()
{
    delete mGameState.AxeKing;
    delete mGameState.Block;
    delete mGameState.map;
    delete mGameState.robber;

    mGameState.AxeKing = nullptr;
    mGameState.Block   = nullptr;
    mGameState.map     = nullptr;
    mGameState.robber = nullptr;
}
