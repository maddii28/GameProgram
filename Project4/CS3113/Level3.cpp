#include "Level3.h"
#include "cs3113.h"

extern AppStatus gAppStatus;
extern int  gLives;
extern bool gShowWin;
extern bool gShowLose;

constexpr Vector2 ATLAS_DIMENSIONS   = { 1.0f, 6.0f };
constexpr Vector2 FLYER_ATLAS_DIMENSIONS = { 1.0f, 6.0f }; // same as others

Level3::Level3()
    : Scene()
{
}

Level3::Level3(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{
}

void Level3::initialise()
{
    // No more levels after this for now
    mGameState.nextSceneID = -1;

    float sizeRatio = 48.0f / 64.0f;

    // --- MAP ---
    mGameState.map = new Map(
        LEVEL_WIDTH,
        LEVEL_HEIGHT,
        mLevelData,
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/marble_packed.png",
        TILE_DIMENSION,
        9, 9,
        mOrigin
    );

    // --- FLYER NPC (reusing robber pointer) ---
    std::map<Direction, std::vector<int>> FlyerAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6 } },
    };

    
    mGameState.flyer = new Entity(
        { mOrigin.x+25  , mOrigin.y  },
        { 100.0f * sizeRatio, 95.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/flying-head.png",
        ATLAS,
        FLYER_ATLAS_DIMENSIONS,
        FlyerAnimationAtlas,
        NPC
    );

    
    mGameState.flyer->setAIType(FOLLOWER);
    mGameState.flyer->setAIState(IDLE);
    mGameState.flyer->setColliderDimensions({
        mGameState.flyer->getScale().x / 3.0f,
        mGameState.flyer->getScale().y / 3.0f
    });

    
    mGameState.flyer->setAcceleration({ 0.0f, 0.0f });


    // --- PLAYER (same as Level2 style) ---
    std::map<Direction, std::vector<int>> AxeKingAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
    };

    mGameState.AxeKing = new Entity(
        { mOrigin.x - 300.0f, mOrigin.y - 250.0f },
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

    // --- PORTAL / EXIT BLOCK ---
    mGameState.Block = new Entity(
        { mOrigin.x - 125, mOrigin.y + 190.0f },
        { 94.0f * sizeRatio, 70.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/portal.png",
        BLOCK
    );

    mGameState.Block->setColliderDimensions({
        mGameState.Block->getScale().x / 3.0f,
        mGameState.Block->getScale().y / 3.0f
    });

    // --- CAMERA ---
    mGameState.camera = {};
    mGameState.camera.target   = mGameState.AxeKing->getPosition();
    mGameState.camera.offset   = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;
}

void Level3::update(float deltaTime)
{

    // Player update
    mGameState.AxeKing->update(
        deltaTime,
        nullptr,
        mGameState.map,
        nullptr,
        0
    );

    
    mGameState.flyer->update(
        deltaTime,
        mGameState.AxeKing,  
        mGameState.map,
        nullptr,
        0
    );


    if (mGameState.AxeKing->isColliding(mGameState.Block))
    {
        gShowWin = true;
        mGameState.nextSceneID = 0;  
    }

    
if (mGameState.AxeKing->isColliding(mGameState.flyer))
{
    gLives--;

    if (gLives > 0)
    {
        mGameState.nextSceneID = 3;  
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
        mGameState.nextSceneID = 3;  
    }
    else
    {
        gShowLose = true;
        mGameState.nextSceneID = 0;
    }
}


    mGameState.camera.target = mGameState.AxeKing->getPosition();
}

void Level3::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.AxeKing->render();
    mGameState.Block->render();
    mGameState.flyer->render();   
}

void Level3::shutdown()
{
    delete mGameState.AxeKing;
    delete mGameState.Block;
    delete mGameState.map;
    delete mGameState.flyer;

    mGameState.AxeKing = nullptr;
    mGameState.Block   = nullptr;
    mGameState.map     = nullptr;
    mGameState.flyer  = nullptr;
}
