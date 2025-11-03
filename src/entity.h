#pragma once

#include <raylib.h>

namespace synthrush {

class GameScene;

class Entity {
   public:
    Entity(GameScene *scene, Vector3 position) : mPosition(position), mScene(scene) {}
    virtual ~Entity() {}

    virtual void Update(float dT) {}
    virtual void Render(float dT) {}

   protected:
    GameScene *mScene;
    Vector3 mPosition;
};

}  // namespace synthrush
