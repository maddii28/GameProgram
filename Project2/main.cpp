/**
* Author: Maddhav Suneja
* Assignment: Pong Clone
* Date due: 2025-10-13, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>
#include <vector>


constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SPEED         = 300,
                SHRINK_RATE   = 100;

constexpr float MAX_AMP       = 5.0f,
                ROTATION_SPEED = 45.0f;

constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  DRAGON_BASE_SIZE = { 80.0f, 80.0f },
                  BACKGROUND_BASE_SIZE = {SCREEN_WIDTH, SCREEN_HEIGHT},
                  FIREBALL_BASE_SIZE = { 25.0f, 25.0f },
                  FIREBALL_POS = { ORIGIN.x, ORIGIN.y},
                  BACKGROUND_POS = { ORIGIN.x, ORIGIN.y},
                  DRAGON1_POS  = { ORIGIN.x/5, ORIGIN.y},
                  DRAGON2_POS = { 9*ORIGIN.x/5, ORIGIN.y};

constexpr char DRAGON1_FP[] = "/Users/sabrina/Downloads/GameProgram/Project2/assets/Dragon1.png"; 
constexpr char DRAGON2_FP[] = "/Users/sabrina/Downloads/GameProgram/Project2/assets/Dragon2.png";
constexpr char FIREBALL_FP[] = "/Users/sabrina/Downloads/GameProgram/Project2/assets/Fireball.png";
constexpr char WINNER1_FP[] = "/Users/sabrina/Downloads/GameProgram/Project2/assets/Winner1.png";
constexpr char WINNER2_FP[] = "/Users/sabrina/Downloads/GameProgram/Project2/assets/Winner2.png";
constexpr char BACKGROUND_FP[] = "/Users/sabrina/Downloads/GameProgram/Project2/assets/Background.jpg";

// Global Variables
AppStatus gAppStatus    = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f,
          gPulseTime     = 0.0f;

Color BGColor = ColorFromHex("#000000ff");

Vector2 gPosition = BACKGROUND_POS,
        gScale = BACKGROUND_BASE_SIZE,

        gDragon1Position = DRAGON1_POS,
        gDragon1Movement = {0.0f, 0.0f},
        gDragon1Scale = DRAGON_BASE_SIZE,

        gDragon2Position = DRAGON2_POS,
        gDragon2Movement = {0.0f, 0.0f},
        gDragon2Scale = DRAGON_BASE_SIZE, 

        gFireballPosition = FIREBALL_POS,
        gFireballMovement = {0.0f, 0.0f},
        gFireballScale = FIREBALL_BASE_SIZE,

        gWinnerPosition = BACKGROUND_POS,
        gWinnerScale = BACKGROUND_BASE_SIZE;

bool ballstatus = false;
bool oneplayermode = false;
int oneplayerdirec = 1;
int Winner = 0;

Texture2D gBackgroundTexture;
Texture2D gDragon1Texture;
Texture2D gDragon2Texture;
Texture2D gFireballTexture;
Texture2D gWinner1;
Texture2D gWinner2;


void initialise();
void processInput();
void update();
void render();
void shutdown();

bool isColliding(const Vector2 *postionA,  const Vector2 *scaleA, 
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabs(postionA->x - positionB->x) - 
                      ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(postionA->y - positionB->y) - 
                      ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

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

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project2");

    gBackgroundTexture = LoadTexture(BACKGROUND_FP);
    gDragon1Texture = LoadTexture(DRAGON1_FP);
    gDragon2Texture = LoadTexture(DRAGON2_FP);
    gFireballTexture = LoadTexture(FIREBALL_FP);
    gWinner1 = LoadTexture(WINNER1_FP);
    gWinner2 = LoadTexture(WINNER2_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    gDragon1Movement = {0.0f, 0.0f};
    gDragon2Movement = {0.0f, 0.0f};

    if (IsKeyPressed(KEY_A))
        Winner = 0;
           
    //To start the game(i.e get ball moving)
    if (IsKeyPressed(KEY_SPACE) && (gFireballPosition.x == ORIGIN.x) && (gFireballPosition.y == ORIGIN.y) && (Winner == 0)){
        gFireballMovement.x = 1;
        gFireballMovement.y = 0.5;
        ballstatus = true;
    }

    // Dragon1: Q - Up, A- Down
    if (IsKeyDown(KEY_W) && (gDragon1Position.y - gDragon1Scale.y/2 > 0.0f))
        gDragon1Movement.y = -1;  
    else if (IsKeyDown(KEY_S) && (gDragon1Position.y + gDragon1Scale.y/2 < SCREEN_HEIGHT))
        gDragon1Movement.y = 1;   
    
    //input for the one player mode 
    if ((IsKeyPressed(KEY_T)) && (!ballstatus)){
        oneplayermode = true;
    }

    // Dragon2: P - Up, L - Down 
    if (!oneplayermode) {
        if (IsKeyDown(KEY_UP) && (gDragon2Position.y - gDragon2Scale.y/2 > 0.0f)) {
            gDragon2Movement.y = -1;
        } else if (IsKeyDown(KEY_DOWN) && (gDragon2Position.y + gDragon2Scale.y/2 < SCREEN_HEIGHT)) {
            gDragon2Movement.y = 1;
        } else {
            gDragon2Movement.y = 0;
        }
    }
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += 1.0f * deltaTime;

    //Dragon1 movement logic 
    gDragon1Position = {
        gDragon1Position.x,
        gDragon1Position.y + SPEED * gDragon1Movement.y * deltaTime
    };

    // one player mode dragon2 movement
    if (oneplayermode) {
        if ((ballstatus) && (gFireballMovement.x == 1)){
            if (gFireballPosition.y > gDragon2Position.y + gDragon2Scale.y/2)
                gDragon2Movement.y = 1;
            else if (gFireballPosition.y < gDragon2Position.y - gDragon2Scale.y/2)
                gDragon2Movement.y = -1;
            else 
                gDragon2Movement.y = 0;
        } else {
            gDragon2Movement.y = float(oneplayerdirec);
        }
    }

    //Dragon2 movement logic
    gDragon2Position = {
        gDragon2Position.x,
        gDragon2Position.y + SPEED * gDragon2Movement.y * deltaTime
    };

    if (gDragon2Position.y - gDragon2Scale.y / 2.0f <= 0.0f) 
        oneplayerdirec = 1;
    else if (gDragon2Position.y + gDragon2Scale.y / 2.0f >= SCREEN_HEIGHT) 
        oneplayerdirec = -1;

    //Fireball logic for resetting game for left, right walls 
    if (gFireballPosition.x + gFireballScale.x/2 > SCREEN_WIDTH || gFireballPosition.x - gFireballScale.x/2 < 0.0f){
        if (gFireballPosition.x + gFireballScale.x/2 > SCREEN_WIDTH)
            Winner = 1;
        else
            Winner = -1;
        ballstatus = false;
        oneplayermode = false;
        gFireballPosition = {ORIGIN.x, ORIGIN.y};
    }

    //Fireball movement logic during active gameplay
    if (ballstatus){
        gFireballPosition = {
            gFireballPosition.x + ((175 * SPEED)/100) * gFireballMovement.x * deltaTime,
            gFireballPosition.y + SPEED * gFireballMovement.y * deltaTime
        };
    }else{gFireballMovement = {0.0f,0.0f};}
    
    //Fireball logic for bouncing off the top n down wall
    if (gFireballPosition.y - gFireballScale.y/2 < 0.0f){
        gFireballMovement.y = 0.5f;
    } 
    else if (gFireballPosition.y + gFireballScale.y/2 > SCREEN_HEIGHT)
        gFireballMovement.y = -0.5f;


    //colliding and bouncing off logic for dragon1 and dragon2
    if (isColliding(&gDragon2Position, &gDragon2Scale, &gFireballPosition, &gFireballScale)) {
        gFireballMovement.x = -1; 
        if (gFireballPosition.y < ((gDragon2Position.y + gDragon2Scale.y/2) - 2*gDragon2Scale.y/3)){
            gFireballMovement.y = -0.5f; 
        }
        else if (gFireballPosition.y > ((gDragon2Position.y + gDragon2Scale.y/2) - gDragon2Scale.y/3)){
            gFireballMovement.y = 0.5f;
        }
        else{
            gFireballMovement.y = 0.0f;
        }
    }  
    else if (isColliding(&gDragon1Position, &gDragon1Scale, &gFireballPosition, &gFireballScale)) {
        gFireballMovement.x = 1; 
        if (gFireballPosition.y < ((gDragon1Position.y + gDragon1Scale.y/2) - 2*gDragon1Scale.y/3)){
            gFireballMovement.y = -0.5f; 
        }
        else if (gFireballPosition.y > ((gDragon1Position.y + gDragon1Scale.y/2) - gDragon1Scale.y/3)){
            gFireballMovement.y = 0.5f;
        }
        else{
            gFireballMovement.y = 0.0f;
        }
    }  
}

void render()
{
    BeginDrawing();
    ClearBackground(BGColor);

    renderObject(&gBackgroundTexture, &gPosition, &gScale);
    renderObject(&gDragon1Texture, &gDragon1Position, &gDragon1Scale);
    renderObject(&gDragon2Texture, &gDragon2Position, &gDragon2Scale);
    renderObject(&gFireballTexture, &gFireballPosition, &gFireballScale);

    if (Winner == 1)
        renderObject(&gWinner1, &gWinnerPosition, &gWinnerScale);
    else if (Winner == -1)
        renderObject(&gWinner2, &gWinnerPosition, &gWinnerScale);

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