/**
* Author: Maddhav Suneja
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>
#include <vector>

// Global Constants
constexpr int   RADIUS          = 110.0f,
                MOON_RADIUSX     = 175.0f,        
                MOON_RADIUSY     = 125.f,
                SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SPEED         = 6,
                SHRINK_RATE   = 100;
constexpr float MAX_AMP       = 5.0f,
                ORBIT_SPEED     = 1.0f,
                MOON_ORBIT_SPEED = 2.0f,
                ROTATION_SPEED = 45.0f;

constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  MOON_BASE_SIZE = { 100.0f, 100.0f },
                  EARTH_BASE_SIZE   = { 190.0f, 190.0f },
                  ROCKET_BASE_SIZE = { 40.0f, 40.0f },
                  MOON_INIT_POS  = { ORIGIN.x, ORIGIN.y - 130.0f },
                  EARTH_INIT_POS    = { ORIGIN.x + 125.0f, ORIGIN.y + 80.0f },
                  ROCKET_INIT_POS = { ORIGIN.x, ORIGIN.y};

constexpr char EARTH_FP[] = "/Users/sabrina/Downloads/GameProgram/Project1/assets/Earth.png"; 
constexpr char MOON_FP[] = "/Users/sabrina/Downloads/GameProgram/Project1/assets/Moon.png";
constexpr char ROCKET_FP[] = "/Users/sabrina/Downloads/GameProgram/Project1/assets/Rocket.png";


// Global Variables
AppStatus gAppStatus    = RUNNING;
float     gAngle         = 0.0f,
          gMoonAngle     = 0.0f,
          gRocketAngle   = 0.0f,
          gRotationAngle = 0.0f,
          gPreviousTicks = 0.0f,
          gPulseTime     = 0.0f;
Color BGColor = ColorFromHex("#000000ff");

Vector2 gPosition = EARTH_INIT_POS,
        gScale    = EARTH_BASE_SIZE,

        gMoonPosition = MOON_INIT_POS,
        gMoonScale    = MOON_BASE_SIZE,

        gRocketPosition = ROCKET_INIT_POS,
        gRocketScale = ROCKET_BASE_SIZE;

Texture2D gEarthTexture;
Texture2D gMoonTexture;
Texture2D gRocketTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Hey Professor Cruz, Thx for this function - it was really handy for render
void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale, float gAngle = 0.0f)
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

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project1");

    gEarthTexture = LoadTexture(EARTH_FP);
    gMoonTexture = LoadTexture(MOON_FP);
    gRocketTexture = LoadTexture(ROCKET_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += 1.0f * deltaTime;

    //earth's scaling
    gScale = {
        EARTH_BASE_SIZE.x + MAX_AMP * cos(SPEED * gPulseTime),
        EARTH_BASE_SIZE.y + MAX_AMP * cos(SPEED * gPulseTime)
    };

    //earth's circular orbit translation pattern
    gAngle += ORBIT_SPEED * deltaTime;
    gPosition.x  = ORIGIN.x + RADIUS * cos(gAngle);
    gPosition.y  = ORIGIN.y + RADIUS * sin(gAngle);

    //moon's elliptical orbit translation pattern
    gMoonAngle += MOON_ORBIT_SPEED * deltaTime;
    gMoonPosition.x = gPosition.x + MOON_RADIUSX * cos(gMoonAngle);
    gMoonPosition.y = gPosition.y + MOON_RADIUSY * sin(gMoonAngle);

    //earth's and moon rotation
    gRotationAngle += ROTATION_SPEED * deltaTime;

    //rocket movement[Infinity like motion btw earth n moon]
    gRocketAngle += 2.0f * deltaTime;
    gRocketPosition.x = (gPosition.x + gMoonPosition.x)/2 + 0.5*(gMoonPosition.x - gPosition.x)*cos(gRocketAngle); //using midpoint as the centre 
    gRocketPosition.y = (gPosition.y + gMoonPosition.y)/2 + 0.5*(gMoonPosition.y - gPosition.y)*sin(gRocketAngle);


    //half orbit - blue, full orbit - black
    if (gAngle > 2*PI) gAngle -= 2*PI;
    if (gAngle >= PI && gAngle <= PI + 0.3)      //Extra Credit YAYY lol
    {
        BGColor = ColorFromHex("#020418ff");
    } else if (gAngle >= 2*PI - 0.3 && gAngle <= 2*PI)
    {
        BGColor = ColorFromHex("#000000ff");
    }
    
}

void render()
{
    BeginDrawing();
    ClearBackground(BGColor);

    //render the earth
    renderObject(&gEarthTexture, &gPosition, &gScale, gRotationAngle);

    //render the moon
    renderObject(&gMoonTexture, &gMoonPosition, &gMoonScale, gRotationAngle);

    //render the rocket 
    renderObject(&gRocketTexture, &gRocketPosition, &gRocketScale);

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