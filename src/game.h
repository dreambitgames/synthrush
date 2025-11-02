#pragma once

#include <raylib.h>

namespace synthrush {

class Game {
   public:
    Game();
    ~Game();

    void Update(float dT);
    void Render(float dT);

    bool IsRunning() { return mRunning; }

   private:
    bool mRunning = true;
    Camera3D mCam{};
};

}  // namespace synthrush
