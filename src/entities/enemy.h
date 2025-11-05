#pragma once

#include "../entity.h"

namespace synthrush {

class Enemy : public Entity {
   public:
    Enemy(GameScene *gs, Vector3 pos);
    ~Enemy();

    virtual void Update(float dT) override;
    virtual void Render(float dT) override;

   private:
    float mEntryModifier = 0;
    Color mColor;
};

}  // namespace synthrush
