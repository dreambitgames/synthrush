#include <raylib.h>

#include <cstdio>

int main() {
    InitWindow(800, 450, "[SYNTHRUSH]");

    Camera3D camera{};
    camera.position = {0, 10, 10};
    camera.target = {0, 0, 0};
    camera.up = {0, 1, 0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode3D(camera);
        DrawCube({0, 0, 0}, 2, 2, 2, RED);
        EndMode3D();
        EndDrawing();
    }

    return 0;
}
