#ifndef G_PROJECTILE_H
#define G_PROJECTILE_H

#include "./../../HaGame/HaGame.h"
#include "Bullet.h"

struct Projectile {
	Bullet bullet;
	Vec2 direction;
	float aliveFor;
	uint32_t ownerId;
};

#endif
