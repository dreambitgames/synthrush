#include "enemy.h"

#include <raylib.h>

synthrush::Enemy::Enemy(Vector3 pos) : Entity(pos) { mColor = BLUE; }

synthrush::Enemy::~Enemy() {}

void synthrush::Enemy::Update(float dT) { mPosition.z -= dT * 10; }

void synthrush::Enemy::Render(float dT) { DrawSphereWires(mPosition, 3, 7, 7, mColor); }
