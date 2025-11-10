#include "CS3113/Entity.h"

struct GameState
{
    Entity *AxeKing;
    Entity *Block;

    Map *map;

    Music bgm;
    Sound jumpSound;

    Camera2D camera;
};

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[]      = "#011627";
constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  ATLAS_DIMENSIONS = { 6, 8 };

constexpr int   NUMBER_OF_TILES         = 20,
                NUMBER_OF_BLOCKS        = 3;
constexpr float TILE_DIMENSION          = 75.0f,
                // in m/ms², since delta time is in ms
                ACCELERATION_OF_GRAVITY = 981.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f,
                END_GAME_THRESHOLD      = 800.0f;

constexpr int LEVEL_WIDTH  = 14,
              LEVEL_HEIGHT = 13;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;


unsigned int gLevelData[] = {
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    4,0,0,0,0,0,0,4,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,4,0,0,4,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,4,0,0,0,0,4,0,4,
    4,0,0,0,4,0,0,0,0,0,4,0,0,4,
    4,4,0,0,0,0,0,0,4,4,0,0,0,4,
    4,0,0,0,0,4,4,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,4,0,4,4,2,2,4,
    4,0,0,0,0,0,0,0,0,4,4,4,4,4,
    4,2,2,2,2,2,2,2,4,4,4,4,4,4,
    4,3,3,3,3,3,3,3,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,
};


GameState gState;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maps");
    InitAudioDevice();

    gState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
    SetMusicVolume(gState.bgm, 0.33f);
    // PlayMusicStream(gState.bgm);

    gState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

    /*
        ----------- MAP -----------
    */
    gState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
        (unsigned int *) gLevelData, // grid data
        "assets/game/tileset.png",   // texture filepath
        TILE_DIMENSION,              // tile size
        4, 1,                        // texture cols & rows
        ORIGIN                       // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
        {DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 }},
        {LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 }},
        {UP,    { 24, 25, 26, 27, 28, 29, 30, 31 }},
        {RIGHT, { 40, 41, 42, 43, 44, 45, 46, 47 }},
    };

    float sizeRatio  = 48.0f / 64.0f;

    // Assets from @see https://sscary.itch.io/the-adventurer-female
    gState.AxeKing = new Entity(
        {ORIGIN.x - 300.0f, ORIGIN.y - 200.0f}, // position
        {200.0f * sizeRatio, 200.0f},           // scale
        "assets/game/walk.png",                 // texture file address
        ATLAS,                                  // single image or atlas?
        ATLAS_DIMENSIONS,                       // atlas dimensions
        xochitlAnimationAtlas,                  // actual atlas
        PLAYER                                  // entity type
    );

    gState.Block = new Entity(
        {90, 220}, 
        {80.0f, 80.0f},
        "/Users/sabrina/Downloads/GameProgram/Class08/assets/game/tile_0061.png", 
        BLOCK 
    );

    gState.AxeKing->setJumpingPower(550.0f);
    gState.AxeKing->setColliderDimensions({
        gState.AxeKing->getScale().x / 3.5f,
        gState.AxeKing->getScale().y / 3.0f
    });
    gState.AxeKing->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    /*
        ----------- CAMERA -----------
    */
    gState.camera = { 0 };                                // zero initialize
    gState.camera.target = gState.AxeKing->getPosition(); // camera follows player
    gState.camera.offset = ORIGIN;                        // camera offset to center of screen
    gState.camera.rotation = 0.0f;                        // no rotation
    gState.camera.zoom = 1.0f;                            // default zoom

    SetTargetFPS(FPS);
}

void processInput() 
{
    gState.AxeKing->resetMovement();

    if      (IsKeyDown(KEY_A)) gState.AxeKing->moveLeft();
    else if (IsKeyDown(KEY_D)) gState.AxeKing->moveRight();

    if (IsKeyPressed(KEY_W) && gState.AxeKing->isCollidingBottom())
    {
        gState.AxeKing->jump();
        PlaySound(gState.jumpSound);
    }

    if (GetLength(gState.AxeKing->getMovement()) > 1.0f) 
        gState.AxeKing->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        UpdateMusicStream(gState.bgm);

        gState.AxeKing->update(
            FIXED_TIMESTEP, // delta time / fixed timestep
            nullptr,        // player
            gState.map,     // map
            nullptr,
            0               // col. entity count
        );

        deltaTime -= FIXED_TIMESTEP;

        Vector2 currentPlayerPosition = gState.AxeKing->getPosition();

        panCamera(&gState.camera, &currentPlayerPosition);

        if (gState.AxeKing->getPosition().y > 800.0f) gAppStatus = TERMINATED;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    BeginMode2D(gState.camera);

    gState.AxeKing->render();
    gState.map->render();
    gState.Block ->render();

    EndMode2D();

    EndDrawing();
}

void shutdown() 
{
    delete gState.AxeKing;
    delete gState.map;

    UnloadMusicStream(gState.bgm);
    UnloadSound(gState.jumpSound);

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