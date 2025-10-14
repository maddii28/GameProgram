#include "CS3113/cs3113.h"
#include <math.h>

enum Member { MURDOC = 0, TWO_D = 1, RUSSEL = 2, NOODLE = 3 };

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SIZE          = 200,
                FRAME_LIMIT   = 100;
constexpr float MAX_AMP       = 10.0f;

constexpr char    BG_COLOUR[] = "#000000";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };

// Image owned by Gorillaz @see https://gorillaz.com/
constexpr char ALBUM_COVER_FP[] = "/Users/sabrina/Downloads/GameProgram/Exercise2/assets/demon_days.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gPulseTime     = 0.0f,
          gPreviousSwap  = 0.0f,
          gPreviousTick  = 0.0f,
          gFrameCounter  = 0.0f;
Vector2   gPosition      = ORIGIN;
Vector2   gScale         = BASE_SIZE;
Member    gCurrentMember = MURDOC;
Texture2D gTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gTexture = LoadTexture(ALBUM_COVER_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    /**
     * @todo Calculate delta time
     */
    float time = (float) GetTime();
    float deltaTime = time - gPreviousTick;
    gPreviousTick = time;

    /**
     * @todo Apply delta time to the time-dependent logic
     */
    gPulseTime += 1.0f * deltaTime;

    gScale = {
        BASE_SIZE.x + MAX_AMP * cos(gPulseTime),
        BASE_SIZE.y + MAX_AMP * cos(gPulseTime)
    };

    /**
     * @todo Switch member every 100 fames
     */
    gFrameCounter ++;
    if (gFrameCounter > 100){
        gFrameCounter = 0;
        
        switch (gCurrentMember)
        {   
        case MURDOC:
            gCurrentMember = TWO_D;
            break;
        case TWO_D:
            gCurrentMember = RUSSEL;
            break;
        case RUSSEL:
            gCurrentMember = NOODLE;
            break;
        case NOODLE:
            gCurrentMember = MURDOC;
            break;
        default:
            break;
        }
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    /**
     * @todo Design your UV coordinates (i.e. textureArea) so that only one
     * member is being rendered onto the screen.
     */
    Rectangle textureArea = {
    static_cast<float>((gCurrentMember % 2) * (gTexture.width  / 2.0f)),
    static_cast<float>((gCurrentMember / 2) * (gTexture.height / 2.0f)),

    // width/height of the area (as floats)
    static_cast<float>(gTexture.width)  / 2.0f,
    static_cast<float>(gTexture.height) / 2.0f
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        gPosition.x,
        gPosition.y,
        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 objectOrigin = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        gTexture, 
        textureArea, 
        destinationArea, 
        objectOrigin, 
        gAngle, 
        WHITE
    );

    EndDrawing();
}

void shutdown() { CloseWindow(); }

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