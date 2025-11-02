#include <raylib.h>

#include "game.h"

int main() {
    synthrush::Game game;

    while (game.IsRunning() && !WindowShouldClose()) {
        float dT = GetFrameTime();
        game.Update(dT);
        game.Render(dT);
    }

    return 0;
}
