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
        mCurrentScene->Initialize();
    }

    void ChangeSceneTransition(Scene *newScene);

    bool IsRunning() { return mRunning; }

    void EndGame() { mRunning = false; }

    int screenW, screenH;
    int virtualW, virtualH;

    Font mainFont;

    Color bgColor{48, 3, 80, 255};
    Color fgColor{233, 52, 121, 255};
    Color fg2Color{21, 60, 180, 255};

   private:
    bool mSceneTransitioning = false;
    Scene *mSceneTransitioningTo = nullptr;
    float mSceneTransitioningTime = 0;
    const float mSceneTransitioningMaxTime = 1.0f;

    bool mRunning = true;
    Scene *mCurrentScene = nullptr;
};

}  // namespace synthrush
