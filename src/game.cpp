#include "game.h"

#include <raylib.h>

#include <cmath>

synthrush::Game::Game() {
    InitWindow(1920, 1080, "[SYNTHRUSH]");

    mCam.position = {0, 5, -10};
    mCam.target = {0, 5, 0};
    mCam.up = {0, 1, 0};
    mCam.fovy = 45;
    mCam.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
}

synthrush::Game::~Game() { CloseWindow(); }

void synthrush::Game::Update(float dT) {}

static void DrawGroundGrid(float off) {
    const float gridSpacing = 3.0f;
    const int gridCount = 30;
    const int gridCountSides = 10;
    const float halfWidth = (gridCountSides / 2.0f) * gridSpacing;

    off = fmodf(off, gridSpacing);

    for (int i = -gridCountSides / 2; i <= gridCountSides / 2; ++i) {
        float x = i * gridSpacing;
        DrawLine3D({x, 0, -gridSpacing * gridCount}, {x, 0, gridSpacing * gridCount}, MAGENTA);
    }

    for (int i = -gridCount; i <= gridCount; ++i) {
        float z = i * gridSpacing - off;
        DrawLine3D({-halfWidth, 0, z}, {halfWidth, 0, z}, MAGENTA);
    }
}

void synthrush::Game::Render(float dT) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(mCam);

    static float off = 0;
    DrawGroundGrid(off);
    off += dT * 10;

    EndMode3D();
    EndDrawing();
}
