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

    virtual void Destroy() { toBeDeleted = true; }

    bool NeedsDeletion() { return toBeDeleted; }

   protected:
    bool toBeDeleted = false;
    GameScene *mScene;
    Vector3 mPosition;
};

}  // namespace synthrush
