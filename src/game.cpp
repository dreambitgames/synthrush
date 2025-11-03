#include "game.h"

#include <raylib.h>
#include <raymath.h>

#include <cmath>

#include "scenes/gameScene.h"

synthrush::Game::Game(int w, int h) : screenW(w), screenH(h) {
    InitWindow(w, h, "[SYNTHRUSH]");
    ToggleFullscreen();
    SetTargetFPS(60);

    ChangeScene(new GameScene(this));
}

synthrush::Game::~Game() { CloseWindow(); }

void synthrush::Game::Update(float dT) { mCurrentScene->Update(dT); }

void synthrush::Game::Render(float dT) {
    BeginDrawing();
    mCurrentScene->Render(dT);
    EndDrawing();
}
