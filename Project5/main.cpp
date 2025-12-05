#include "raylib.h"
#include "CS3113/Entity.h"
#include <vector>
#include <map>

constexpr int SCREEN_WIDTH  = 900;
constexpr int SCREEN_HEIGHT = 600;

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
constexpr float ARROW_SPEED    = 250.0f;

constexpr float HIT_ZONE_MIN = 80;
constexpr float HIT_ZONE_MAX = 140;

AppStatus gAppStatus = RUNNING;

Texture2D gBackground;

Music gMusic;
float gSongLength = 0;
float gSongTime = 0;
bool gGameEnded = false;
Sound gHitSound;
Sound gHitSound2;
Sound gHitSound3;
Sound gHitSound4;


Texture2D gTexUp;
Texture2D gTexDown;
Texture2D gTexLeft;
Texture2D gTexRight;

Entity* gPlayer = nullptr;
Entity* gAI     = nullptr;

int gScore = 0;
int gAIScore = 0;


float gPrevTicks = 0;
float gAccumulator = 0;

float laneX[4] = { 300, 400, 500, 600 };

Direction laneDir[4] = { LEFT, DOWN, UP, RIGHT };


struct Arrow {
    Entity e;
    Direction dir;
    int lane;
    bool active;
    bool aiScored;
};

int gCurrentLyricIndex = -1;
struct LyricBlock {
    float time;
    std::vector<std::string> lines;
};
std::vector<LyricBlock> gLyrics = {

    { 27.0f, {
        "Uh-huh, this my ship",
        "All the girls stomp your feet like this",
        "A few times I've been around this track",
        "And like that I'm gone and I'm lightning fast"
    }},

    { 33.0f, {
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl",
        "A few times I've been around this track",
        "And like that I'm gone and I'm lightning fast"
    }},

    { 39.0f, {
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl",
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship"
    }},

    { 47.6f, {
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship",
        "I heard that you were doggin' ships",
        "And you didn't think that I would hear it"
    }},

    { 56.8f, {
        "People hear you braggin' like that",
        "Get the engine binders fired up",
        "So I'm ready to attack, gonna lead the pack",
        "Gonna make you touch down"
    }},


    { 64.4f, {
        "Gonna take you out",
        "That's right, put your blasters down",
        "Get the engine binders fired up",
        "A few times I've been around this track"
    }},

  
    { 71.2f, {
        "And like that I'm gone and I'm lightning fast",
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl",
        "A few times I've been around this track"
    }},

    { 79.2f, {
        "And like that I'm gone and I'm lightning fast",
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl",
        "Ooh, this my ship, this my ship"
    }},


    { 87.2f, {
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship",
        "So that's right Hutt, meet me at the track",
        "No dirty tricks, no hidden attack"
    }},


    { 96.2f, {
        "Both of us want to be the winner",
        "But there can only be one",
        "So I'm gonna fly, gonna give it my all",
        "Gonna make you fall, you cheeska slimo"
    }},


    { 105.6f, {
        "That's right, I'm the last one standing",
        "Another one bites the dust",
        "A few times I've been around this track",
        "And like that I'm gone and I'm lightning fast"
    }},


    { 112.0f, {
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl",
        "A few times I've been around this track",
        "And like that I'm gone and I'm lightning fast"
    }},


    { 120.0f, {
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl",
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship"
    }},


    { 128.0f, {
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship",
        "This ship beat Sebulba",
        "S-E-B-U-L-B-A"
    }},


    { 135.6f, {
        "This ship beat Sebulba",
        "S-E-B-U-L-B-A",
        "This ship beat Sebulba",
        "S-E-B-U-L-B-A"
    }},


    { 142.0f, {
        "A few times I've been around this track",
        "And like that I'm gone and I'm lightning fast",
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl"
    }},


    { 150.2f, {
        "A few times I've been around this track",
        "And like that I'm gone and I'm lightning fast",
        "'Cause I ain't no hologram girl",
        "I ain't no hologram girl"
    }},


    { 158.0f, {
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship",
        "Ooh, this my ship, this my ship"
    }}
    };

std::vector<Arrow> gArrows(24);
float gArrowSpawnTimer = 0;


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dance-Off Game - Final DDR Build");
    SetTargetFPS(60);


    // Load background
    gBackground = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/Arena1.png");

    InitAudioDevice();
    gMusic = LoadMusicStream("/Users/sabrina/Downloads/GameProgram/Project5/assets/17. Hologram Girl.mp3");
    PlayMusicStream(gMusic);
    SetMusicVolume(gMusic, 0.7f);
    gHitSound = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/123.wav");
    SetSoundVolume(gHitSound, 0.9f);

    gHitSound2 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/456.mp3");
    SetSoundVolume(gHitSound2, 0.9f);

    gHitSound3 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/789.wav");
    SetSoundVolume(gHitSound3, 0.9f);

    gHitSound4 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/10.wav");
    SetSoundVolume(gHitSound4, 0.9f);

    gSongLength = GetMusicTimeLength(gMusic);
    gSongTime = 0;

    // Load arrow images
    gTexUp    = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/UP.png");
    gTexDown  = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/DOWN.png");
    gTexLeft  = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/LEFT.png");
    gTexRight = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/RIGHT.png");

    // PLAYER ATLAS
    Vector2 dim = {4,8};
    std::map<Direction, std::vector<int>> playerAtlas = {
        {IDLE, {24}},
        {UP,    {0,1,2,3,4,5,6,7}},
        {DOWN,  {8,9,10,11,12,13,14,15}},
        {LEFT,  {16,17,18,19,20,21,22,23}},
        {RIGHT, {24,25,26,27,28,29,30,31}}
    };

    gPlayer = new Entity(
        {300, 480},
        {150, 150},
        "assets/User.png",
        ATLAS,
        dim,
        playerAtlas,
        PLAYER,
        ANIMATION_ONLY
    );

    // AI ATLAS
    Vector2 dimAI = {4,4};
    std::map<Direction, std::vector<int>> aiAtlas = {
        {IDLE, {0}},
        {UP,    {0,1,2,3}},
        {DOWN,  {4,5,6,7}},
        {LEFT,  {8,9,10,11}},
        {RIGHT, {12,13,14,15}}
    };

    gAI = new Entity(
        {620, 480},
        {150, 150},
        "assets/AI3.png",
        ATLAS,
        dimAI,
        aiAtlas,
        NPC,
        ANIMATION_ONLY
    );


    for (auto &a : gArrows) {
        a.e = Entity(
            {500, 500}, 
            {80,80}, 
            "assets/UP.png", 
            PLAYER, 
            ANIMATION_ONLY
        );
        a.e.deactivate();
        a.active = false;
        a.lane = 0;
        a.dir = UP;
    }
}


void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
        gAppStatus = TERMINATED;

    if (IsKeyPressed(KEY_UP))    gPlayer->setDirection(UP);
    if (IsKeyPressed(KEY_DOWN))  gPlayer->setDirection(DOWN);
    if (IsKeyPressed(KEY_LEFT))  gPlayer->setDirection(LEFT);
    if (IsKeyPressed(KEY_RIGHT)) gPlayer->setDirection(RIGHT);

    if (!IsKeyDown(KEY_UP) &&
        !IsKeyDown(KEY_DOWN) &&
        !IsKeyDown(KEY_LEFT) &&
        !IsKeyDown(KEY_RIGHT))
    {
        gPlayer->setDirection(IDLE);
    }
}


void update()
{
    float ticks = GetTime();
    float delta = ticks - gPrevTicks;
    gPrevTicks = ticks;

    delta += gAccumulator;
    if (delta < FIXED_TIMESTEP) {
        gAccumulator = delta;
        return;
    }
    if (!gGameEnded)
        UpdateMusicStream(gMusic);

    if (!gGameEnded)
        gSongTime += FIXED_TIMESTEP;

    if (gSongTime >= gSongLength && !gGameEnded)
    {
        gGameEnded = true;
        StopMusicStream(gMusic);
    }

    if (gCurrentLyricIndex + 1 < gLyrics.size()) {
        if (gSongTime >= gLyrics[gCurrentLyricIndex + 1].time) {
            gCurrentLyricIndex++;
        }
    }

    while (delta >= FIXED_TIMESTEP)
    {
        if (gGameEnded){
        delta -= FIXED_TIMESTEP;
        continue;  
        }

        float songProgress = gSongTime / gSongLength; 
        float arrowSpeed = ARROW_SPEED * (1.0f + 1.5f * songProgress);
        float spawnRate = 0.75f * (1.0f - 0.6f * songProgress);
        if (spawnRate < 0.20f) spawnRate = 0.20f; 

        gArrowSpawnTimer += FIXED_TIMESTEP;
        if (gArrowSpawnTimer > spawnRate) {
            gArrowSpawnTimer = 0;

            int lane = GetRandomValue(0,3);
            Direction dir = laneDir[lane];

            for (auto &a : gArrows) {
                if (!a.active) {
                    a.lane = lane;
                    a.dir = dir;

                    switch (dir) {
                        case IDLE: break;
                        case UP:    a.e.setSprite(gTexUp);    break;
                        case DOWN:  a.e.setSprite(gTexDown);  break;
                        case LEFT:  a.e.setSprite(gTexLeft);  break;
                        case RIGHT: a.e.setSprite(gTexRight); break;
                    }

                    a.e.setPosition({laneX[lane], 520 });
                    a.e.activate();
                    a.active = true;
                    a.aiScored = false; 
                    break;
                }
            }
        }

        for (auto &a : gArrows)
        {
            if (!a.active) continue;

            Vector2 pos = a.e.getPosition();
            pos.y -= arrowSpeed * FIXED_TIMESTEP;
            a.e.setPosition(pos);

            if (pos.y >= HIT_ZONE_MIN && pos.y <= HIT_ZONE_MAX)
            {
                if ((a.dir == LEFT  && IsKeyDown(KEY_LEFT)) ||
                    (a.dir == DOWN  && IsKeyDown(KEY_DOWN)) ||
                    (a.dir == UP    && IsKeyDown(KEY_UP)) ||
                    (a.dir == RIGHT && IsKeyDown(KEY_RIGHT)))
                {
                    gScore++;
                    if (a.dir == LEFT  && IsKeyDown(KEY_LEFT)){
                        PlaySound(gHitSound); 
                    }
                    else if (a.dir == DOWN  && IsKeyDown(KEY_DOWN)){
                        PlaySound(gHitSound2); 
                    }
                    else if (a.dir == UP  && IsKeyDown(KEY_UP)){
                        PlaySound(gHitSound3); 
                    }
                    else if (a.dir == RIGHT && IsKeyDown(KEY_RIGHT)){
                        PlaySound(gHitSound4); 
                    }
                    a.active = false;
                    a.e.deactivate();
                    continue;
                }
            }

            // MISS
            if (pos.y < HIT_ZONE_MIN - 40) {
                a.active = false;
                a.e.deactivate();
            }
        }
        
        for (auto &a : gArrows){
            if (!a.active || a.aiScored) continue;
            Vector2 pos = a.e.getPosition();
            if (pos.y >= HIT_ZONE_MIN && pos.y <= HIT_ZONE_MAX){
                Direction aiMove;
                if (a.dir == UP || a.dir == DOWN){
                    aiMove = a.dir; 
                }
                else{
                    int coinFlip = GetRandomValue(0, 1);
                    aiMove = (coinFlip == 0 ? LEFT : RIGHT);
                }
                gAI->setDirection(aiMove);
                
                if (aiMove == a.dir)
                    gAIScore++;

                a.aiScored = true; 

                break; 
            }
        }

        gPlayer->update(FIXED_TIMESTEP, nullptr, nullptr, nullptr, 0);
        gAI->update(FIXED_TIMESTEP, nullptr, nullptr, nullptr, 0);


        delta -= FIXED_TIMESTEP;
    }

    gAccumulator = delta;
}


void render()
{
    BeginDrawing();

    ClearBackground(BLACK);

    DrawTexture(gBackground, 0, 0, WHITE);

    DrawText(TextFormat("Score: %d", gScore), 30, 30, 40, YELLOW);
    DrawText(TextFormat("AI Score: %d", gAIScore), 630, 30, 40, ORANGE);


    int x = 220;
    int y = HIT_ZONE_MIN;
    int w = 480;int h = 60;
    int t = 6;  
    
    DrawRectangle(x, y, w, t, BLACK);
    DrawRectangle(x, y + h - t, w, t, BLACK);
    DrawRectangle(x, y, t, h, BLACK);
    DrawRectangle(x + w - t, y, t, h, BLACK);

    gPlayer->render();
    gAI->render();

    if (gCurrentLyricIndex >= 0 && gCurrentLyricIndex < gLyrics.size()) {
        const auto &block = gLyrics[gCurrentLyricIndex];

        int startY = 240;   // lower area of the screen
        int lineSpacing = 28;

        for (int i = 0; i < block.lines.size(); i++) {
            DrawText(block.lines[i].c_str(),
                230,
                startY + i * lineSpacing,
                26,
                WHITE);
        }
    }

    // Draw arrows
    for (auto &a : gArrows)
        if (a.active)
            a.e.render();
    
    
    if (gGameEnded){
        const char* winnerText;

        if (gScore > gAIScore)
            winnerText = "YOU WIN!";
        else if (gAIScore > gScore)
            winnerText = "AI WINS!";
        else
            winnerText = "TIE GAME!";

        DrawRectangle(200, 200, 500, 200, Fade(BLACK, 0.7f));

        DrawText(winnerText, 300, 260, 50, YELLOW);
        DrawText("Press Q to Quit", 275, 330, 30, WHITE);
    }

    EndDrawing();
}

void shutdown()
{
    UnloadSound(gHitSound);
    UnloadSound(gHitSound2);
    UnloadSound(gHitSound3);
    UnloadSound(gHitSound4);
    CloseWindow();
}


int main()
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
