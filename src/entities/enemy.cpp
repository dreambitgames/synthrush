#include "enemy.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <cmath>

#include "../random.h"
#include "../scenes/gameScene.h"

synthrush::Enemy::Enemy(GameScene *gs, Vector3 pos, int beatN) : beatN(beatN), Entity(gs, pos) {
    mEnemyType = (Type)round(util::Random(0, 1));

    switch (mEnemyType) {
        case Type::BLUE_SPHERE:
            mColor = BLUE;
            break;
        case Type::GREEN_BOX:
            mColor = GREEN;
    }

    mColor.a = 0;
}

synthrush::Enemy::~Enemy() {}

void synthrush::Enemy::Update(float dT) {
    mPosition.z -= dT * mScene->mapMoveSpeed;

    mEntryModifier = Lerp(mEntryModifier, 1, 5 * dT);

    if (mPosition.z <= 1) {
        Destroy();
        mScene->OnEnemyMissed(beatN);
    }
}

void synthrush::Enemy::Render(float dT) {
    rlPushMatrix();
    rlTranslatef(mPosition.x, mPosition.y + 5 - 5 * mEntryModifier, mPosition.z);
    rlRotatef(GetTime() * 100, 1, 1, sinf(GetTime()));

    float markedCoefficient = mMarked ? 1 : 0.3;

    Color lineColor = Fade(mColor, mEntryModifier * markedCoefficient);
    Color fillColor = Fade(lineColor, 0.6f);

    Vector3 center = {0, 0, 0};
    float size = 1.0f;
    constexpr int sphereRings = 7;
    constexpr int sphereSlices = 7;

    switch (mEnemyType) {
        case Type::BLUE_SPHERE:
            DrawSphereWires(center, size, sphereRings, sphereSlices, lineColor);
            DrawSphere(center, size, fillColor);
            break;

        case Type::GREEN_BOX:
            DrawCubeWires(center, size, size, size, lineColor);
            DrawCube(center, size, size, size, fillColor);
            break;
    }

    rlPopMatrix();
}

void synthrush::Enemy::CheckHit(Ray ray) {
    RayCollision r = GetRayCollisionSphere(ray, mPosition, 1);
    if (!r.hit)
        return;

    Destroy();
    mScene->OnEnemyShot(beatN);
}

void synthrush::Enemy::Destroy() { Entity::Destroy(); }
