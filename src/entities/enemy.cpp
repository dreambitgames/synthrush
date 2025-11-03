#include "enemy.h"

#include <raylib.h>
#include <rlgl.h>

#include <cmath>

synthrush::Enemy::Enemy(Vector3 pos) : Entity(pos) { mColor = BLUE; }

synthrush::Enemy::~Enemy() {}

void synthrush::Enemy::Update(float dT) { mPosition.z -= dT * 10; }

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
