#include "Level1.h"
#include "cs3113.h"

extern AppStatus gAppStatus;
extern int  gLives;
extern bool gShowWin;
extern bool gShowLose;


constexpr Vector2 ATLAS_DIMENSIONS   = { 1.0f, 6.0f };
constexpr Vector2 SALTLAS_DIMENSIONS = { 1.0f, 16.0f };

Level1::Level1()
    : Scene()
{
}

Level1::Level1(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{
}

void Level1::initialise()
{
    
    mGameState.nextSceneID = -1;

    float sizeRatio = 48.0f / 64.0f;

    
    mGameState.map = new Map(
        LEVEL_WIDTH,
        LEVEL_HEIGHT,
        mLevelData,
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/marble_packed.png",
        TILE_DIMENSION,
        9, 9,
        mOrigin
    );
    
    std::map<Direction, std::vector<int>> SamuraiAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7,
                   8, 9, 10,11,12,13,14,15,16 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7,
                   8, 9, 10,11,12,13,14,15,16 } },
    };

    mGameState.samurai = new Entity(
        { mOrigin.x, mOrigin.y - 200.0f },
        { 220.0f * sizeRatio, 152.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/Samurai.png",
        ATLAS,
        SALTLAS_DIMENSIONS,
        SamuraiAnimationAtlas,
        NPC
    );

    mGameState.samurai->setAIType(FOLLOWER);
    mGameState.samurai->setAIState(IDLE);
    mGameState.samurai->setColliderDimensions({
        mGameState.samurai->getScale().x / 3.0f,
        mGameState.samurai->getScale().y / 3.0f
    });
    mGameState.samurai->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.samurai->setDirection(LEFT);
    mGameState.samurai->setSpeed(7*Entity::DEFAULT_SPEED/10);

    //AXE KING (PLAYER)
    std::map<Direction, std::vector<int>> AxeKingAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
    };

    mGameState.AxeKing = new Entity(
        { mOrigin.x - 300.0f, mOrigin.y - 200.0f },
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
        7.0f * mGameState.AxeKing->getScale().y / 10.0f
    });
    mGameState.AxeKing->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    // BLOCK (PORTAL / TRIGGER) 
    mGameState.Block = new Entity(
        { mOrigin.x + 238.0f, mOrigin.y - 320.0f },
        { 94.0f * sizeRatio, 70.0f },
        "/Users/sabrina/Downloads/GameProgram/Project4/assets/portal.png",
        BLOCK
    );

    mGameState.Block->setColliderDimensions({
        mGameState.Block->getScale().x / 3.0f,
        mGameState.Block->getScale().y / 3.0f
    });

    // CAMERA 
    mGameState.camera = {};
    mGameState.camera.target   = mGameState.AxeKing->getPosition();
    mGameState.camera.offset   = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;
}

void Level1::update(float deltaTime)
{

    // Player update
    mGameState.AxeKing->update(
        deltaTime,
        nullptr,
        mGameState.map,
        nullptr,
        0
    );

    // Samurai AI update
    mGameState.samurai->update(
        deltaTime,
        mGameState.AxeKing,
        mGameState.map,
        nullptr,
        0
    );


    if (mGameState.AxeKing->isColliding(mGameState.Block))
    {
        mGameState.nextSceneID = 2; 
    }
    
    if (mGameState.AxeKing->isColliding(mGameState.samurai))
    {
        gLives--;
        
        if (gLives > 0)
        {
            mGameState.nextSceneID = 1;
        }else{
            gShowLose = true;
            mGameState.nextSceneID = 0;  
        }
    }
    
    if (mGameState.AxeKing->getPosition().y > END_GAME_THRESHOLD)
    {
        gLives--;
        
        if (gLives > 0){
        mGameState.nextSceneID = 1;  
        }else{
        gShowLose = true;
        mGameState.nextSceneID = 0;  
        }
    }


    
    mGameState.camera.target = mGameState.AxeKing->getPosition();

}

void Level1::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.AxeKing->render();
    mGameState.Block->render();
    mGameState.samurai->render();
}

void Level1::shutdown()
{
    delete mGameState.AxeKing;
    delete mGameState.Block;
    delete mGameState.map;
    delete mGameState.samurai;


    mGameState.AxeKing = nullptr;
    mGameState.Block   = nullptr;
    mGameState.map     = nullptr;
    mGameState.samurai = nullptr;
}
