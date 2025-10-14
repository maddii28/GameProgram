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

        gWinnerPosition = BACKGROUND_POS,
        gWinnerScale = BACKGROUND_BASE_SIZE;

struct Fireball {
    Vector2 position;
    Vector2 movement;
    Vector2 scale;
};

Fireball gFireball1 = {
    FIREBALL_POS,          
    {0.0f, 0.0f},          
    FIREBALL_BASE_SIZE     
};

Fireball gFireball2 = {
    FIREBALL_POS,          
    {0.0f, 0.0f},          
    FIREBALL_BASE_SIZE     
};

Fireball gFireball3 = {
    FIREBALL_POS,          
    {0.0f, 0.0f},          
    FIREBALL_BASE_SIZE     
};

bool gballstatus = false;
bool goneplayermode = false;
int goneplayerdirec = 1;
int gWinner = 0;
int gBallCount = 1;
int gBallsOut = 0;

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

void updateball(Fireball* ball, float deltaTime){
    
    if (ball->movement.x == 0.0f && ball->movement.y == 0.0f) return;

    //fireball movement
    ball->position = {
        ball->position.x + ((175 * SPEED)/100) * ball->movement.x * deltaTime,
        ball->position.y + SPEED * ball->movement.y * deltaTime
    };
    
    //up and down walls bounce off logic
    if (ball->position.y - ball->scale.y/2 < 0.0f){
        ball->movement.y = 0.5f;
    }else if(ball->position.y + ball->scale.y/2 > SCREEN_HEIGHT)
        ball->movement.y = -0.5f;

    
    //the fireball bouncing off the dragon logic(divided dragon into 3 parts and did diff fireball movement)
    if (isColliding(&gDragon2Position, &gDragon2Scale, &ball->position, &ball->scale)) {
        ball->movement.x = -1.0f; 
        if (ball->position.y < ((gDragon2Position.y + gDragon2Scale.y/2) - 2*gDragon2Scale.y/3)){
            ball->movement.y = -0.5f; 
        }
        else if (ball->position.y > ((gDragon2Position.y + gDragon2Scale.y/2) - gDragon2Scale.y/3)){
            ball->movement.y = 0.5f;
        }
        else{
            ball->movement.y = 0.0f;
        }
    }else if (isColliding(&gDragon1Position, &gDragon1Scale, &ball->position, &ball->scale)) {
        ball->movement.x = 1.0f; 
        if (ball->position.y < ((gDragon1Position.y + gDragon1Scale.y/2) - 2*gDragon1Scale.y/3)){
            ball->movement.y = -0.5f; 
        }
        else if (ball->position.y > ((gDragon1Position.y + gDragon1Scale.y/2) - gDragon1Scale.y/3)){
            ball->movement.y = 0.5f;
        }
        else{
            ball->movement.y = 0.0f;
        }
    }  

    //ball hitting the left or right side of wall logic
    if (ball->position.x - ball->scale.x/2 > SCREEN_WIDTH || ball->position.x + ball->scale.x/2 < 0.0f){   

        ball->movement = {0.0f, 0.0f};
        
        int pointWinner;
        if (ball->position.x + ball->scale.x/2 > SCREEN_WIDTH)
            pointWinner = 1; 
        else if (ball->position.x - ball->scale.x/2 < 0.0f)
            pointWinner = -1; 
            
        gBallsOut += 1;
        
        if (gBallsOut == gBallCount) {
            gWinner = pointWinner; 
            gballstatus = false;
            goneplayermode = false;   
            gBallsOut = 0; 
        }
    }
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

    if (gWinner == 0 && !gballstatus){
        if (IsKeyPressed(KEY_TWO))
            gBallCount = 2;
        else if (IsKeyPressed(KEY_THREE))
            gBallCount = 3;
    }

    if (IsKeyPressed(KEY_A)){
        gWinner = 0;
        gBallCount = 1;

        gFireball1.position = FIREBALL_POS;
        gFireball2.position = FIREBALL_POS;
        gFireball3.position = FIREBALL_POS;
    }
      
    //To start the game(i.e get ball moving)
    if (IsKeyPressed(KEY_SPACE) && !gballstatus && (gWinner == 0)){

        gBallsOut = 0;

        gFireball1.movement.x = 1.0;
        gFireball1.movement.y = 0.5;

        //Launch Ball 2 
        if (gBallCount >= 2){
            gFireball2.movement.x = 1.0;
            gFireball2.movement.y = -0.5; 
        }else{
            gFireball2.movement = {0.0f, 0.0f}; 
        }
        
        // Launch Ball 3 
        if (gBallCount == 3) {
            gFireball3.movement.x = 1.0;
            gFireball3.movement.y = 0.0; 
        } else {
            gFireball3.movement = {0.0f, 0.0f};
        }

        gballstatus = true;
    }

    // Dragon1: Q - Up, A- Down
    if (IsKeyDown(KEY_W) && (gDragon1Position.y - gDragon1Scale.y/2 > 0.0f))
        gDragon1Movement.y = -1;  
    else if (IsKeyDown(KEY_S) && (gDragon1Position.y + gDragon1Scale.y/2 < SCREEN_HEIGHT))
        gDragon1Movement.y = 1;   
    
    //input for the one player mode 
    if ((IsKeyPressed(KEY_T)) && (!gballstatus)){
        goneplayermode = true;
    }

    // Dragon2: P - Up, L - Down 
    if (!goneplayermode) {
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
    if (goneplayermode) {
        if ((gballstatus) && (gFireball1.movement.x == 1)){
            if (gFireball1.position.y > gDragon2Position.y + gDragon2Scale.y/2)
                gDragon2Movement.y = 1;
            else if (gFireball1.position.y < gDragon2Position.y - gDragon2Scale.y/2)
                gDragon2Movement.y = -1;
            else 
                gDragon2Movement.y = 0;
        } else {
            gDragon2Movement.y = float(goneplayerdirec);
        }
    }

    //Dragon2 movement logic
    gDragon2Position = {
        gDragon2Position.x,
        gDragon2Position.y + SPEED * gDragon2Movement.y * deltaTime
    };

    //to keep dragon2 in screen bounds for one player logic
    if (gDragon2Position.y - gDragon2Scale.y / 2.0f <= 0.0f) 
        goneplayerdirec = 1;
    else if (gDragon2Position.y + gDragon2Scale.y / 2.0f >= SCREEN_HEIGHT) 
        goneplayerdirec = -1;

    if (gballstatus){
        updateball(&gFireball1, deltaTime);

        if (gBallCount >= 2) 
            updateball(&gFireball2, deltaTime);
        
        if (gBallCount == 3) 
            updateball(&gFireball3, deltaTime);
    }
    
}

void render()
{
    BeginDrawing();
    ClearBackground(BGColor);

    renderObject(&gBackgroundTexture, &gPosition, &gScale);
    renderObject(&gDragon1Texture, &gDragon1Position, &gDragon1Scale);
    renderObject(&gDragon2Texture, &gDragon2Position, &gDragon2Scale);

    if (gBallCount >= 1)
        renderObject(&gFireballTexture, &gFireball1.position, &gFireball1.scale);
    
    if (gBallCount >= 2)
        renderObject(&gFireballTexture, &gFireball2.position, &gFireball2.scale);
    
    if (gBallCount >= 3)
        renderObject(&gFireballTexture, &gFireball3.position, &gFireball3.scale);

    if (gWinner == 1)
        renderObject(&gWinner1, &gWinnerPosition, &gWinnerScale);
    else if (gWinner == -1)
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