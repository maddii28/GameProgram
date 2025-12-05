#include "raylib.h"
#include "CS3113/Scene.h"
#include "CS3113/Level1.h"
#include "CS3113/Level2.h"

Scene* currentScene = nullptr;

void SwitchScene(int id) {
    delete currentScene;

    if (id == 1) currentScene = new Level1();
    if (id == 2) currentScene = new Level2();

    currentScene->init();
}

int main() {
    InitWindow(900, 600, "Dance-Off Game");
    SetTargetFPS(60);

    SwitchScene(1); // start on menu

    while (!WindowShouldClose()) {
        currentScene->processInput();
        currentScene->update(GetFrameTime());
        currentScene->render();

        if (currentScene->finished) {
            SwitchScene(currentScene->nextScene);
        }
    }

    delete currentScene;
    CloseWindow();
    return 0;
}
