#pragma once

#include <raylib.h>

#include <random>

#include "entities/enemy.h"
#include "scene.h"

namespace synthrush {

class Game {
   public:
    Game(int w, int h, int vw, int vh);
    ~Game();

    void Update(float dT);
    void Render(float dT);

    void ChangeScene(Scene *newScene) {
        if (mCurrentScene)
            delete mCurrentScene;
        mCurrentScene = newScene;
    }

    bool IsRunning() { return mRunning; }

    void EndGame() { mRunning = false; }

    int screenW, screenH;
    int virtualW, virtualH;

    Font mainFont;

   private:
    bool mRunning = true;
    Scene *mCurrentScene = nullptr;
};

}  // namespace synthrush
