#ifndef G_ENEMY_SYSTEM_H
#define G_ENEMY_SYSTEM_H

#include "./../../HaGame/HaGame.h"

class EnemySystem : public hagame::ecs::System {
public:
	String getSystemName() {
		return "EnemySystem";
	}
};

#endif