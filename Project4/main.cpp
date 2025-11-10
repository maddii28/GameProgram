// main.cpp
#include "CS3113/Level0.h"
#include "CS3113/Level1.h"
#include "CS3113/Level2.h"
#include "CS3113/Level3.h"

// Global constants
constexpr int SCREEN_WIDTH  = 1000;
constexpr int SCREEN_HEIGHT = 600;
constexpr int FPS           = 120;

int  gLives    = 3;      
bool gShowWin  = false;  
bool gShowLose = false; 

Music gBgm; 
Sound gJumpSound;   
Sound gLoseSound; 
Sound gWinSound;

constexpr Vector2 ORIGIN = {
    SCREEN_WIDTH  / 2.0f,
    SCREEN_HEIGHT / 2.0f
};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Globals
AppStatus gAppStatus = RUNNING;
extern bool gShowLose; 

float gPreviousTicks   = 0.0f;
float gTimeAccumulator = 0.0f;

// Scene globals
Scene*              gCurrentScene = nullptr;
std::vector<Scene*> gScenes;

Level0* gLevel0 = nullptr;
Level1* gLevel1 = nullptr;
Level2* gLevel2 = nullptr;
Level3* gLevel3 = nullptr;

// Function declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void switchToScene(Scene* scene);

void switchToScene(Scene* scene)
{
    gCurrentScene = scene;
    if (gCurrentScene) gCurrentScene->initialise();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 4 - Scenes");
    InitAudioDevice();
    SetTargetFPS(FPS);

    gBgm = LoadMusicStream("/Users/sabrina/Downloads/GameProgram/Project4/assets/Call to Adventure.mp3");
    SetMusicVolume(gBgm, 0.33f);
    PlayMusicStream(gBgm); 

    gJumpSound   = LoadSound("/Users/sabrina/Downloads/GameProgram/Project4/assets/jump-up-245782.mp3");
    gLoseSound   = LoadSound("/Users/sabrina/Downloads/GameProgram/Project4/assets/losing-horn-313723.mp3");
    gWinSound   = LoadSound("/Users/sabrina/Downloads/GameProgram/Project4/assets/victorymale-version-230553.mp3");

    gLevel0 = new Level0(ORIGIN, "#000000");
    gLevel1 = new Level1(ORIGIN, "#C0897E");
    gLevel2 = new Level2(ORIGIN, "#011627");
    gLevel3 = new Level3(ORIGIN, "#102A43");

    gScenes.push_back(gLevel0); 
    gScenes.push_back(gLevel1); 
    gScenes.push_back(gLevel2); 
    gScenes.push_back(gLevel3);

    switchToScene(gScenes[0]); 
}

void processInput()
{
    if (!gCurrentScene) return;

    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
    {
        gAppStatus = TERMINATED;
        return;
    }

    Entity* player = gCurrentScene->getState().AxeKing;
    if (!player) return;

    if (!player) return;

    player->resetMovement();

    if (IsKeyDown(KEY_A))      player->moveLeft();
    else if (IsKeyDown(KEY_D)) player->moveRight();

    if (IsKeyPressed(KEY_W) && player->isCollidingBottom())
    {
        player->jump();
        PlaySound(gJumpSound);
    }

    if (GetLength(player->getMovement()) > 1.0f)
    {
        player->normaliseMovement();
    }
}

void update()
{
    UpdateMusicStream(gBgm);

    if (!gCurrentScene) return;

    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime;

    int nextID = gCurrentScene->getState().nextSceneID;
    
    if (nextID >= 0 && nextID < static_cast<int>(gScenes.size())){

        if (nextID == 0 && gShowLose)
        {
            PlaySound(gLoseSound);
        }
        else if (gShowWin)
        {
            PlaySound(gWinSound);
        }

        switchToScene(gScenes[nextID]);
    }
}

void render()
{
    if (!gCurrentScene) return;

    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown()
{
    
    if (gLevel0) { gLevel0->shutdown(); delete gLevel0; gLevel0 = nullptr; }
    if (gLevel1) { gLevel1->shutdown(); delete gLevel1; gLevel1 = nullptr; }
    if (gLevel2) { gLevel2->shutdown(); delete gLevel2; gLevel2 = nullptr; }
    if (gLevel3) { gLevel3->shutdown(); delete gLevel3; gLevel3 = nullptr; }

    gScenes.clear();
    gCurrentScene = nullptr;

    UnloadSound(gJumpSound);
    UnloadSound(gWinSound);
    UnloadMusicStream(gBgm);
    UnloadSound(gLoseSound);

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
