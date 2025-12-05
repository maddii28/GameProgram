#include "Level1.h"

void Level1::init() {
    bg = LoadTexture("/Users/sabrina/Downloads/GameProgram/Project5/assets/Menu.png"); 
    finished = false;
    nextScene = -1;
}

void Level1::processInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        finished = true;
        nextScene = 2; 
    }
}

void Level1::update(float dt) {
}

void Level1::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(bg, 0, 0, WHITE);

    EndDrawing();
}
