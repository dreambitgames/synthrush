#pragma once

namespace synthrush {

class Game;

class Scene {
   public:
    Scene(Game *game) : mGame(game) {}
    virtual ~Scene() {}

    virtual void Initialize() {}

    virtual void Update(float dT) {}
    virtual void Render(float dT) {}

   protected:
    Game *mGame;
};

}  // namespace synthrush
