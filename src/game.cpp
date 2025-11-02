#include "game.h"

#include <raylib.h>
#include <raymath.h>

#include <cmath>

synthrush::Game::Game(int w, int h) : mScreenW(w), mScreenH(h) {
    InitWindow(w, h, "[SYNTHRUSH]");
    ToggleFullscreen();

    mCam.position = {0, 5, -10};
    mCam.target = {0, 5, 0};
    mCam.up = {0, 1, 0};
    mCam.fovy = 45;
    mCam.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
}

synthrush::Game::~Game() { CloseWindow(); }

void synthrush::Game::Update(float dT) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mousePosNormalied = {2 * mousePos.x / mScreenW - 1, 2 * mousePos.y / mScreenH - 1};
    Vector3 camTarget = {-mousePosNormalied.x, 5 - mousePosNormalied.y, 0};

    mCam.target = Vector3Lerp(mCam.target, camTarget, 10 * dT);

    mLastRay = GetMouseRay(mousePos, mCam);
    mLastRay.position = {0, 0, 0};
}

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

#include <string>

void synthrush::Game::Render(float dT) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(mCam);

    static float off = 0;
    DrawGroundGrid(off);
    off += dT * 10;

    DrawRay(mLastRay, BLUE);

    EndMode3D();

    DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 32, WHITE);
    EndDrawing();
}
