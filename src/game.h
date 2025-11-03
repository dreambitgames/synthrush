#pragma once

#include <raylib.h>

#include "entities/enemy.h"

namespace synthrush {

class Game {
   public:
    Game(int w, int h);
    ~Game();

    void Update(float dT);
    void Render(float dT);

    bool IsRunning() { return mRunning; }

   private:
    Enemy mTestEnemy;

    Ray mLastRay;

    int mScreenW, mScreenH;

    bool mRunning = true;
    Camera3D mCam{};
};

}  // namespace synthrush
