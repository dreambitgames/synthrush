#include "enemy.h"

#include <raylib.h>
#include <rlgl.h>

#include <cmath>

#include "../scenes/gameScene.h"

synthrush::Enemy::Enemy(GameScene *gs, Vector3 pos) : Entity(gs, pos) { mColor = BLUE; }

synthrush::Enemy::~Enemy() {}

void synthrush::Enemy::Update(float dT) { mPosition.z -= dT * mScene->mapMoveSpeed; }

void synthrush::Enemy::Render(float dT) {
    rlPushMatrix();
    rlTranslatef(mPosition.x, mPosition.y, mPosition.z);
    rlRotatef(GetTime() * 100, 1, 1, sinf(GetTime()));

    DrawSphereWires({0, 0, 0}, 1, 7, 7, mColor);
    DrawSphere(
        {
            0,
            0,
            0,
        },
        1, Fade(mColor, 0.6));

    rlPopMatrix();
}
