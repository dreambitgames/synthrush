#pragma once

#include <array>

#include "../entity.h"

namespace synthrush {

class Enemy : public Entity {
   public:
    enum class Type { SPHERE, BOX };

    Enemy(GameScene *gs, Vector3 pos, int beatN);
    ~Enemy();

    virtual void Update(float dT) override;
    virtual void Render(float dT) override;

    void SetMarked() { mMarked = true; }

    void CheckHit(Ray ray);

    virtual void Destroy() override;

    int beatN;

   private:
    Type mEnemyType;

    bool mMarked = false;

    float mEntryModifier = 0;
    Color mColor;
};

}  // namespace synthrush
