#include "Level0.h"
#include "cs3113.h"   // this should bring in raylib symbols like DrawText, WHITE

extern AppStatus gAppStatus;
extern int  gLives;
extern bool gShowWin;
extern bool gShowLose;

Level0::Level0()
    : Scene()
{
}

Level0::Level0(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{
}

void Level0::initialise()
{
    
    mGameState.nextSceneID = -1;

    
    mGameState.camera = {};
    mGameState.camera.target   = mOrigin;  
    mGameState.camera.offset   = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;

    gLives = 3;
}

void Level0::update(float /*deltaTime*/)
{
    
    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = 1;

        gShowWin  = false;
        gShowLose = false;
    }
}

void Level0::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    if (gShowWin)
    {
        DrawText("YOU WIN!",
                 (int)mOrigin.x - 130, (int)mOrigin.y - 140,
                 40, WHITE);
    }
    else if (gShowLose)
    {
        DrawText("YOU LOSE!",
                 (int)mOrigin.x - 140, (int)mOrigin.y - 140,
                 40, WHITE);
    }


    DrawText("AXE KING ADVENTURE",
             (int)mOrigin.x - 225, (int)mOrigin.y - 80,
             40, WHITE);

    DrawText("Press ENTER to start",
             (int)mOrigin.x - 185, (int)mOrigin.y,
             30, WHITE);

    DrawText("Press Q to quit",
             (int)mOrigin.x - 115, (int)mOrigin.y + 60,
             20, WHITE);
}

void Level0::shutdown()
{
    
}
