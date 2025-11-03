#include "gameScene.h"

#include <raymath.h>

#include <string>

#include "../game.h"

synthrush::GameScene::GameScene(Game *game) : Scene(game) {
    mCam.position = {0, 5, -10};
    mCam.target = {0, 5, 0};
    mCam.up = {0, 1, 0};
    mCam.fovy = 45;
    mCam.projection = CAMERA_PERSPECTIVE;
}

synthrush::GameScene::~GameScene() {}

void synthrush::GameScene::Update(float dT) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mousePosNormalied = {2 * mousePos.x / mGame->screenW - 1,
                                 2 * mousePos.y / mGame->screenH - 1};
    Vector3 camTarget = {-mousePosNormalied.x, 5 - mousePosNormalied.y, 0};

    mCam.target = Vector3Lerp(mCam.target, camTarget, 10 * dT);
    // UpdateCamera(&mCam, CAMERA_FREE);

    mShootRay = GetMouseRay(mousePos, mCam);
    mShootRay.position = {0, 0, 0};
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

void synthrush::GameScene::Render(float dT) {
    ClearBackground(BLACK);
    BeginMode3D(mCam);

    static float off = 0;
    DrawGroundGrid(off);
    off += dT * mapMoveSpeed;

    DrawRay(mShootRay, BLUE);

    EndMode3D();

    DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 32, WHITE);
}
