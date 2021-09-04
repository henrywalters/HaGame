#ifndef G_TURRET_H
#define G_TURRET_H

#include "./../Enemy.h"

const EnemyDef TURRET {
	[](Ptr<hagame::ecs::Entity> entity, double dt) {
		std::cout << dt << "\n";
	},
	{ "SEARCH", "ATTACK" },
	120 * DEG_TO_RAD,
	500.0f,
	100.0f,
	0.0f,
	NULL
};

#endif
