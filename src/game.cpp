#include "game.h"

#include <raylib.h>

synthrush::Game::Game() {
    InitWindow(800, 450, "[SYNTHRUSH]");

    mCam.position = {0, 10, 10};
    mCam.target = {0, 0, 0};
    mCam.up = {0, 1, 0};
    mCam.fovy = 45;
    mCam.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
}

synthrush::Game::~Game() { CloseWindow(); }

void synthrush::Game::Update(float dT) { UpdateCamera(&mCam, CAMERA_FREE); }

void synthrush::Game::Render(float dT) {
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode3D(mCam);

    DrawCube({0, 0, 0}, 2, 2, 2, RED);

    EndMode3D();
    EndDrawing();
}
