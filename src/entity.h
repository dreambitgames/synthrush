#pragma once

#include <raylib.h>

namespace synthrush {

class Entity {
   public:
    Entity(Vector3 mPosition) : mPosition(mPosition) {}
    virtual ~Entity() {}

    virtual void Update(float dT) {}
    virtual void Render(float dT) {}

   protected:
    Vector3 mPosition;
};

}  // namespace synthrush
