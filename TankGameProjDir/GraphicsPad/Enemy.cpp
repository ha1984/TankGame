#include "Enemy.h"

Enemy::Enemy() : position(25.0f, 0.0f, 0.0f), direction(-1.0f, 0.0f, 0.0f)
{
}

void Enemy::move(){
	position.x += 0.005f*direction.x;
	position.y += 0.005f*direction.y;
	position.z += 0.005f*direction.z;
}

