#pragma once

#include <raylib.h>

namespace synthrush {

class Game {
   public:
    Game(int w, int h);
    ~Game();

    void Update(float dT);
    void Render(float dT);

    bool IsRunning() { return mRunning; }

   private:
    Ray mLastRay;

    int mScreenW, mScreenH;

    bool mRunning = true;
    Camera3D mCam{};
};

}  // namespace synthrush
