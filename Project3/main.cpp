#include "CS3113/Entity.h"
#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char BG_COLOUR[]    = "#000000ff";
constexpr Vector2  ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                   BACKGROUND_BASE_SIZE = {SCREEN_WIDTH, SCREEN_HEIGHT},
                   BACKGROUND_POS = { ORIGIN.x, ORIGIN.y};

constexpr float ACCELERATION_OF_GRAVITY = 0.008f,
                FIXED_TIMESTEP          = 1.0f / 60.0f;

constexpr char BACKGROUND_FP[] = "/Users/sabrina/Downloads/GameProgram/Project3/assets/Background.jpeg";

// Global Variables
AppStatus gAppStatus   = RUNNING;
GameState gamestate = IDLE;

float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      gThrust = 0.02f,
      gDrag = 0.1f,
      gAngle = 0.0f;

Vector2 gPosition = BACKGROUND_POS,
        gScale = BACKGROUND_BASE_SIZE;

Texture2D gBackgroundTexture;

Entity *Rocket = nullptr;
Entity *LandingPad1 = nullptr;
Entity *LandingPad2 = nullptr;
Entity *LandingPad3 = nullptr;
Entity *Platform = nullptr;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale)
{
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture, 
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project3");

    gBackgroundTexture = LoadTexture(BACKGROUND_FP);

    //explosion
    std::map<Direction, std::vector<int>> animationAtlas = {
        {OPEN,  {0,  1,  2,  3 }},
        {CLOSE, {4,  5,  6,  7 }}
    };

    Rocket = new Entity(
        {450.0f, 125.0f}, // position
        {60.0f, 60.0f},  // scale
        "/Users/sabrina/Downloads/GameProgram/Project3/assets/Rocket.png",        // texture file address
        PLAYER
    );

    LandingPad1 = new Entity(
        {160.0f,530.0f},
        {80.0f, 80.0f},
        "/Users/sabrina/Downloads/GameProgram/Project3/assets/LandingPad.png",
        PAD
    );

    LandingPad2 = new Entity(
        {500.0f,530.0f},
        {80.0f, 80.0f},
        "/Users/sabrina/Downloads/GameProgram/Project3/assets/LandingPad.png",
        PAD
    );

    LandingPad3 = new Entity(
        {950.0f,500.0f},
        {80.0f, 80.0f},
        "/Users/sabrina/Downloads/GameProgram/Project3/assets/LandingPad.png",
        PAD
    );

    Platform = new Entity(
        {300.0f,300.0f},
        {110.0f, 60.0f},
        "/Users/sabrina/Downloads/GameProgram/Project3/assets/Platform.png",
        PLATFORM
    );

    Rocket->setColliderDimensions({
        7*Rocket->getScale().x/10,
        7*Rocket->getScale().y/10
    });
    Rocket->setAcceleration({0.0f, 0.0f}); 

    for (Entity* pad:{LandingPad1, LandingPad2, LandingPad3}){
        pad->setColliderDimensions({
            7*pad->getScale().x/10,
            7*pad->getScale().y/10
        });
    }

    Platform->setColliderDimensions({
        Rocket->getScale().x / 3.0f,
        Rocket->getScale().y / 3.0f
    });
}

void processInput() 
{
    if (IsKeyPressed(KEY_SPACE) && gamestate != PLAYING){
        Rocket->setPosition({450.0f, 125.0f});
        Rocket->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

        Rocket->setVelocity({0.0f, 0.0f});

        Rocket->setFuel(200.0f);
        gamestate = PLAYING;

    }

    if (gamestate == PLAYING){
        if (IsKeyDown(KEY_RIGHT) && Rocket->getFuel() > 0.0f){
            Rocket->setAcceleration({gThrust,ACCELERATION_OF_GRAVITY });
            Rocket->consumeFuel(0.005);
        }
        else if (IsKeyDown(KEY_LEFT)&& Rocket->getFuel() > 0.0f){
            Rocket->setAcceleration({-gThrust,ACCELERATION_OF_GRAVITY });
            Rocket->consumeFuel(0.005);
        }
        else if (IsKeyDown(KEY_DOWN)&& Rocket->getFuel() > 0.0f){
            Rocket->setAcceleration({0.0f,ACCELERATION_OF_GRAVITY + gThrust });
            Rocket->consumeFuel(0.005);
        }
        else if (IsKeyDown(KEY_UP)&& Rocket->getFuel() > 0.0f){
            Rocket->setAcceleration({0.0f,ACCELERATION_OF_GRAVITY - gThrust });
            Rocket->consumeFuel(0.005);
        }
        else {
            Rocket->setAcceleration({0 ,ACCELERATION_OF_GRAVITY});
        }
    }

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

    while (deltaTime >= FIXED_TIMESTEP && gamestate == PLAYING)
    {
        Rocket->update(FIXED_TIMESTEP, Platform, LandingPad1,LandingPad2, LandingPad3, gamestate);  
        Platform->update(FIXED_TIMESTEP, Platform, LandingPad1,LandingPad2, LandingPad3, gamestate);

        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() //fix render 
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    renderObject(&gBackgroundTexture, &gPosition, &gScale);

    Rocket->render();
    LandingPad1->render();
    LandingPad2->render();
    LandingPad3->render();
    Platform->render();

    if (gamestate == MISSION_ACCOMPLISHED){
        DrawText("Mission Accomplished", 400, 200, 25, WHITE);
    }else if (gamestate == MISSION_FAILED){
        DrawText("Mission Failed", 400, 200, 25, WHITE);
    }

    const char* fuelText = TextFormat("Fuel: %.1f", Rocket->getFuel());

    DrawText(fuelText, 675, 40, 25, WHITE);

    EndDrawing();
}

void shutdown() 
{ 
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