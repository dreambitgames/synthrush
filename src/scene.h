#pragma once

namespace synthrush {

class Game;

class Scene {
   public:
    Scene(Game *game) : mGame(game) {}
    virtual ~Scene() {}

    virtual void Initialize() { mSceneInitialized = true; }

    virtual void Update(float dT) {}
    virtual void Render(float dT) {}

   protected:
    bool mSceneInitialized = false;

    Game *mGame;
};

}  // namespace synthrush
