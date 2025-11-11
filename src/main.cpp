#include <raylib.h>

#include "game.h"

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const int downscaleFact = 3;

    const int virtualWidth = screenWidth / downscaleFact;
    const int virtualHeight = screenHeight / downscaleFact;

    synthrush::Game game(screenWidth, screenHeight, virtualWidth, virtualHeight);

    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    double lastTime = GetTime();

    while (game.IsRunning() && !WindowShouldClose()) {
        double currentTime = GetTime();
        float dT = (float)(currentTime - lastTime);

        if (dT >= 0.3)
            dT = 0.3;

        lastTime = currentTime;

        game.Update(dT);
        BeginTextureMode(target);
        game.Render(dT);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            target.texture,
            (Rectangle){0, 0, (float)target.texture.width, -(float)target.texture.height},
            (Rectangle){0, 0, (float)screenWidth, (float)screenHeight}, (Vector2){0, 0}, 0.0f,
            WHITE);

        EndDrawing();
    }

    return 0;
}
