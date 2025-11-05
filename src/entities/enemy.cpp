#include "enemy.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <cmath>

#include "../scenes/gameScene.h"

synthrush::Enemy::Enemy(GameScene *gs, Vector3 pos) : Entity(gs, pos) {
    mColor = BLUE;

    mColor.a = 0;
}

synthrush::Enemy::~Enemy() {}

void synthrush::Enemy::Update(float dT) {
    mPosition.z -= dT * mScene->mapMoveSpeed;

    mEntryModifier = Lerp(mEntryModifier, 1, 5 * dT);
}

void synthrush::Enemy::Render(float dT) {
    rlPushMatrix();
    rlTranslatef(mPosition.x, mPosition.y + 5 - 5 * mEntryModifier, mPosition.z);
    rlRotatef(GetTime() * 100, 1, 1, sinf(GetTime()));

    DrawSphereWires({0, 0, 0}, 1, 7, 7, Fade(mColor, mEntryModifier));
    DrawSphere(
        {
            0,
            0,
            0,
        },
        1, Fade(mColor, mEntryModifier * 0.6f));

    rlPopMatrix();
}
