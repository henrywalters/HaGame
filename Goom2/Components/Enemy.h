#ifndef G_ENEMY_H
#define G_ENEMY_H

#include "../../HaGame/HaGame.h"

using EnemyAIFunc = std::function<void(Ptr<hagame::ecs::Entity>, double)>;

struct EnemyDef {
	EnemyAIFunc ai = [](Ptr<hagame::ecs::Entity> entity, double dt) {};
	Set<String> states = {};
	float sightAngle;
	float sightDistance;
	float health;
	float shield;
	Ptr<hagame::ecs::Entity> target;
};

struct Enemy {
	EnemyDef def;
	Ptr<Weapon> weapon;
	String state;
};

#endif