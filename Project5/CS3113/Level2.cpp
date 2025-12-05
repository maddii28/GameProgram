#include "Level2.h"
#include "raylib.h"
#include "Entity.h"
#include <map>

static constexpr int   SCREEN_WIDTH   = 900;
static constexpr int   SCREEN_HEIGHT  = 600;
static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
static constexpr float ARROW_SPEED    = 250.0f;
static constexpr float HIT_ZONE_MIN   = 80.0f;
static constexpr float HIT_ZONE_MAX   = 140.0f;


static float laneX[4] = { 300, 400, 500, 600 };
static Direction laneDir[4] = { LEFT, DOWN, UP, RIGHT };


Level2::~Level2() {
    
    UnloadSound(hit1);
    UnloadSound(hit2);
    UnloadSound(hit3);
    UnloadSound(hit4);
    UnloadMusicStream(music);

    UnloadTexture(background);
    UnloadTexture(texUp);
    UnloadTexture(texDown);
    UnloadTexture(texLeft);
    UnloadTexture(texRight);

    delete player;
    delete ai;
}


void Level2::init() {
    
    background = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/Arena1.png");

    
    InitAudioDevice(); 
    music = LoadMusicStream("/Users/sabrina/Downloads/GameProgram/Project5/assets/17. Hologram Girl.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.6f);

    hit1 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/123.wav");
    SetSoundVolume(hit1, 1.0f);

    hit2 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/456.mp3");
    SetSoundVolume(hit2, 0.5f);

    hit3 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/789.wav");
    SetSoundVolume(hit3, 1.0f);

    hit4 = LoadSound("/Users/sabrina/Downloads/GameProgram/Project5/assets/10.wav");
    SetSoundVolume(hit4, 1.0f);

    songLength = GetMusicTimeLength(music);
    songTime   = 0.0f;
    gameEnded  = false;

    
    texUp    = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/UP.png");
    texDown  = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/DOWN.png");
    texLeft  = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/LEFT.png");
    texRight = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/RIGHT.png");

    
    Vector2 dim = {4, 8};
    std::map<Direction, std::vector<int>> playerAtlas = {
        {IDLE,  {24}},
        {UP,    {0,1,2,3,4,5,6,7}},
        {DOWN,  {8,9,10,11,12,13,14,15}},
        {LEFT,  {16,17,18,19,20,21,22,23}},
        {RIGHT, {24,25,26,27,28,29,30,31}}
    };

    player = new Entity(
        {300, 480},
        {150, 150},
        "assets/User.png",
        ATLAS,
        dim,
        playerAtlas,
        PLAYER,
        ANIMATION_ONLY
    );

   
    Vector2 dimAI = {4, 4};
    std::map<Direction, std::vector<int>> aiAtlas = {
        {IDLE,  {0}},
        {UP,    {0,1,2,3}},
        {DOWN,  {4,5,6,7}},
        {LEFT,  {8,9,10,11}},
        {RIGHT, {12,13,14,15}}
    };

    ai = new Entity(
        {620, 480},
        {150, 150},
        "assets/AI3.png",
        ATLAS,
        dimAI,
        aiAtlas,
        NPC,
        ANIMATION_ONLY
    );

    // Arrows pool
    arrows.resize(24);
    for (auto &a : arrows) {
        a.e = Entity(
            {500, 500},
            {80, 80},
            "assets/UP.png",
            PLAYER,
            ANIMATION_ONLY
        );
        a.e.deactivate();
        a.active   = false;
        a.lane     = 0;
        a.dir      = UP;
        a.aiScored = false;
    }

    loseShader = LoadShader(0, "/Users/sabrina/Downloads/GameProgram/Project5/shaders/fragment.glsl");
    
    float zero = 0.0f;
    SetShaderValue(loseShader,
               GetShaderLocation(loseShader, "strength"),
               &zero,
               SHADER_UNIFORM_FLOAT);

    arrowSpawnTimer   = 0.0f;
    prevTicks         = GetTime();
    accumulator       = 0.0f;
    score             = 0;
    aiScore           = 0;
    currentLyricIndex = -1;

    
    lyrics = {
        { 28.0f, {
            "Uh-huh, this my ship",
            "All the girls stomp your feet like this",
            "A few times I've been around this track",
            "And like that I'm gone and I'm lightning fast"
        }},
        { 35.0f, {
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl",
            "A few times I've been around this track",
            "And like that I'm gone and I'm lightning fast"
        }},
        { 45.0f, {
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl",
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship"
        }},
        { 58.0f, {
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship",
            "I heard that you were doggin' ships",
            "And you didn't think that I would hear it"
        }},
        { 73.0f, {
            "People hear you braggin' like that",
            "Get the engine binders fired up",
            "So I'm ready to attack, gonna lead the pack",
            "Gonna make you touch down"
        }},
        { 80.0f, {
            "Gonna take you out",
            "That's right, put your blasters down",
            "Get the engine binders fired up",
            "A few times I've been around this track"
        }},
        { 86.5f, {
            "And like that I'm gone and I'm lightning fast",
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl",
            "A few times I've been around this track"
        }},
        { 95.5f, {
            "And like that I'm gone and I'm lightning fast",
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl",
            "Ooh, this my ship, this my ship"
        }},
        { 107.0f, {
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship",
            "So that's right Hutt, meet me at the track",
            "No dirty tricks, no hidden attack"
        }},
        { 146.0f, {
            "Both of us want to be the winner",
            "But there can only be one",
            "So I'm gonna fly, gonna give it my all",
            "Gonna make you fall, you cheeska slimo"
        }},
        { 154.0f, {
            "That's right, I'm the last one standing",
            "Another one bites the dust",
            "A few times I've been around this track",
            "And like that I'm gone and I'm lightning fast"
        }},
        { 162.5f, {
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl",
            "A few times I've been around this track",
            "And like that I'm gone and I'm lightning fast"
        }},
        { 172.5f, {
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl",
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship"
        }},
        { 183.0f, {
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship",
            "This ship beat Sebulba",
            "S-E-B-U-L-B-A"
        }},
        { 198.6f, {
            "This ship beat Sebulba",
            "S-E-B-U-L-B-A",
            "This ship beat Sebulba",
            "S-E-B-U-L-B-A"
        }},
        { 212.0f, {
            "A few times I've been around this track",
            "And like that I'm gone and I'm lightning fast",
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl"
        }},
        { 220.2f, {
            "A few times I've been around this track",
            "And like that I'm gone and I'm lightning fast",
            "'Cause I ain't no hologram girl",
            "I ain't no hologram girl"
        }},
        { 230.0f, {
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship",
            "Ooh, this my ship, this my ship"
        }}, 
        { 250.0f, {
            ""
        }}

    };
}

void Level2::processInput() {
    
    if (IsKeyPressed(KEY_Q)) {
        CloseWindow();  
    }

    if (IsKeyPressed(KEY_UP))    player->setDirection(UP);
    if (IsKeyPressed(KEY_DOWN))  player->setDirection(DOWN);
    if (IsKeyPressed(KEY_LEFT))  player->setDirection(LEFT);
    if (IsKeyPressed(KEY_RIGHT)) player->setDirection(RIGHT);

    if (!IsKeyDown(KEY_UP) &&
        !IsKeyDown(KEY_DOWN) &&
        !IsKeyDown(KEY_LEFT) &&
        !IsKeyDown(KEY_RIGHT)) {
        player->setDirection(IDLE);
    }
}


void Level2::update(float /*dt*/) {
    float ticks = GetTime();
    float delta = ticks - prevTicks;
    prevTicks   = ticks;

    accumulator += delta;

    if (!gameEnded)
        UpdateMusicStream(music);

    if (!gameEnded)
        songTime += delta;

    if (songTime >= songLength && !gameEnded) {
        gameEnded = true;
        StopMusicStream(music);

        if (aiScore > score)
            loseEffect = true; 
    }

    if (loseEffect)
        loseTime += FIXED_TIMESTEP;


    if (!lyrics.empty() && currentLyricIndex + 1 < (int)lyrics.size()) {
        if (songTime >= lyrics[currentLyricIndex + 1].time) {
            currentLyricIndex++;
        }
    }

    while (accumulator >= FIXED_TIMESTEP) {
        if (gameEnded) {
            accumulator -= FIXED_TIMESTEP;
            continue;
        }

        float songProgress = songTime / songLength;
        if (songProgress > 1.0f) songProgress = 1.0f;

        float arrowSpeed = ARROW_SPEED * (1.0f + 1.5f * songProgress);
        float spawnRate  = 0.75f * (1.0f - 0.6f * songProgress);
        if (spawnRate < 0.20f) spawnRate = 0.20f;

        // Spawn arrows
        arrowSpawnTimer += FIXED_TIMESTEP;
        if (arrowSpawnTimer > spawnRate) {
            arrowSpawnTimer = 0.0f;

            int lane = GetRandomValue(0, 3);
            Direction dir = laneDir[lane];

            for (auto &a : arrows) {
                if (!a.active) {
                    a.lane = lane;
                    a.dir  = dir;

                    switch (dir) {
                        case UP:    a.e.setSprite(texUp);    break;
                        case DOWN:  a.e.setSprite(texDown);  break;
                        case LEFT:  a.e.setSprite(texLeft);  break;
                        case RIGHT: a.e.setSprite(texRight); break;
                        case IDLE:
                        default: break;
                    }

                    a.e.setPosition({ laneX[lane], 520 });
                    a.e.activate();
                    a.active   = true;
                    a.aiScored = false;
                    break;
                }
            }
        }

      
        for (auto &a : arrows) {
            if (!a.active) continue;

            Vector2 pos = a.e.getPosition();
            pos.y -= arrowSpeed * FIXED_TIMESTEP;
            a.e.setPosition(pos);

       
            if (pos.y >= HIT_ZONE_MIN && pos.y <= HIT_ZONE_MAX) {
                if ((a.dir == LEFT  && IsKeyDown(KEY_LEFT )) ||
                    (a.dir == DOWN  && IsKeyDown(KEY_DOWN )) ||
                    (a.dir == UP    && IsKeyDown(KEY_UP   )) ||
                    (a.dir == RIGHT && IsKeyDown(KEY_RIGHT))) {

                    score++;

                 
                    if (a.dir == LEFT)       PlaySound(hit1);
                    else if (a.dir == DOWN)  PlaySound(hit2);
                    else if (a.dir == UP)    PlaySound(hit3);
                    else if (a.dir == RIGHT) PlaySound(hit4);

                    a.active = false;
                    a.e.deactivate();
                    continue;
                }
            }

            
            if (pos.y < HIT_ZONE_MIN - 40) {
                a.active = false;
                a.e.deactivate();
            }
        }

        
        for (auto &a : arrows) {
            if (!a.active || a.aiScored) continue;

            Vector2 pos = a.e.getPosition();
            if (pos.y >= HIT_ZONE_MIN && pos.y <= HIT_ZONE_MAX) {
                Direction aiMove;
                if (a.dir == UP || a.dir == DOWN) {
                    aiMove = a.dir;  
                } else {
                    int coinFlip = GetRandomValue(0, 1);
                    aiMove = (coinFlip == 0 ? LEFT : RIGHT);
                }

                ai->setDirection(aiMove);

                if (aiMove == a.dir)
                    aiScore++;

                a.aiScored = true;
                break; // one reaction per frame
            }
        }

        // Update dancer animations
        player->update(FIXED_TIMESTEP, nullptr, nullptr, nullptr, 0);
        ai->update(FIXED_TIMESTEP, nullptr, nullptr, nullptr, 0);

        accumulator -= FIXED_TIMESTEP;
    }
}


void Level2::render() {
    BeginDrawing();

    ClearBackground(BLACK);
    DrawTexture(background, 0, 0, WHITE);

    
    DrawText(TextFormat("Score: %d",   score),   30, 30, 40, YELLOW);
    DrawText(TextFormat("AI Score: %d", aiScore), 615, 30, 40, ORANGE);

   
    int x = 220;
    int y = (int)HIT_ZONE_MIN;
    int w = 480;
    int h = 60;
    int t = 6;

    DrawRectangle(x,y,w, t, BLACK);           
    DrawRectangle(x,y + h - t, w, t, BLACK);           
    DrawRectangle(x,y,t, h, BLACK);           
    DrawRectangle(x + w - t, y,t, h, BLACK);           

    if (loseEffect)
    {
    float strength = 1.0f;
    float timeVal = loseTime;

    SetShaderValue(loseShader,
                   GetShaderLocation(loseShader, "strength"),
                   &strength,
                   SHADER_UNIFORM_FLOAT);

    SetShaderValue(loseShader,
                   GetShaderLocation(loseShader, "time"),
                   &timeVal,
                   SHADER_UNIFORM_FLOAT);

    BeginShaderMode(loseShader);
    }

    player->render();

    if (loseEffect)
        EndShaderMode();

    ai->render();


    if (currentLyricIndex >= 0 && currentLyricIndex < (int)lyrics.size()) {
        const auto &block = lyrics[currentLyricIndex];

        int startY      = 240;
        int lineSpacing = 28;
        int startX      = 230;

        for (int i = 0; i < (int)block.lines.size(); ++i) {
            DrawText(block.lines[i].c_str(),
                     startX,
                     startY + i * lineSpacing,
                     26,
                     WHITE);
        }
    }

    // Arrows
    for (auto &a : arrows)
        if (a.active)
            a.e.render();


    // End-of-song result
    if (gameEnded) {
        const char* winnerText;

        if (score > aiScore)
            winnerText = "YOU WIN!";
        else if (aiScore > score)
            winnerText = "AI WINS!";
        else
            winnerText = "TIE GAME!";

        DrawRectangle(200, 200, 500, 200, Fade(BLACK, 0.7f));
        DrawText(winnerText,330, 260, 50, YELLOW);
        DrawText("Press Q to Quit", 300, 330, 30, WHITE);
    }

    EndDrawing();
}
