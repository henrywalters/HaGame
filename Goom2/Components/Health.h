#ifndef G_HEALTH_H
#define G_HEALTH_H

#include "./../../HaGame/HaGame.h"

struct Health {
	float maxHealth = 100.0f;
	float health = 100.0f;
	float maxShield = 100.0f;
	float shield = 0.0f;

	bool destroyOnDead = true;

	bool isAlive() {
		return health > 0;
	}

	void damage(float dmg) {
		if (dmg < shield) {
			shield -= dmg;
		}
		else if (dmg > shield && shield > 0) {
			dmg -= shield;
			shield = 0.0f;
		}
		else if (dmg < health) {
			health -= dmg;
		}
		else {
			health = 0.0f;
		}
	}

	void addHealth(float amount) {
		health = clamp<float>(health + amount, 0, maxHealth);
	}

	void addShield(float amount) {
		shield = clamp<float>(shield + amount, 0, maxShield);
	}
};

#endif
