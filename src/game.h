#pragma once

#include <raylib.h>

#include <random>

#include "entities/enemy.h"
#include "scene.h"

namespace synthrush {

class Game {
   public:
    Game(int w, int h);
    ~Game();

    void Update(float dT);
    void Render(float dT);

    void ChangeScene(Scene *newScene) {
        if (mCurrentScene)
            delete mCurrentScene;
        mCurrentScene = newScene;
    }

    bool IsRunning() { return mRunning; }

    float Random(float min, float max);

    int screenW, screenH;

   private:
    void EndGame() { mRunning = false; }

    std::mt19937 mRng;

    bool mRunning = true;
    Scene *mCurrentScene = nullptr;
};

}  // namespace synthrush
