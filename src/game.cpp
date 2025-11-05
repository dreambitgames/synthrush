#include "game.h"

#include <raylib.h>
#include <raymath.h>

#include <cmath>
#include <fstream>
#include <random>

#include "levelData.h"
#include "scenes/gameScene.h"

synthrush::Game::Game(int w, int h) : screenW(w), screenH(h) {
    InitWindow(w, h, "[SYNTHRUSH]");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();

    std::ifstream f("data/levels/test.json");
    json data = json::parse(f);
    LevelData testLevel(data);

    ChangeScene(new GameScene(this, testLevel));
}

synthrush::Game::~Game() {
    if (mCurrentScene)
        delete mCurrentScene;
    CloseWindow();
}

void synthrush::Game::Update(float dT) { mCurrentScene->Update(dT); }

void synthrush::Game::Render(float dT) {
    BeginDrawing();
    mCurrentScene->Render(dT);
    EndDrawing();
}
