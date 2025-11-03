#pragma once

#include "../entity.h"

namespace synthrush {

class Enemy : public Entity {
   public:
    Enemy(Vector3 pos);
    ~Enemy();

    virtual void Update(float dT) override;
    virtual void Render(float dT) override;

   private:
    Color mColor;
};

}  // namespace synthrush
