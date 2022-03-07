#ifndef WEAPON_CONTROLLER_H
#define WEAPON_CONTROLLER_H

#include "../Common/Weapons.h"

struct WeaponController {
	Weapon weapon;
	bool canFire = true;
	double timeSinceLastFire = 0.0;

	int clipAmmo;
	int totalAmmo;

	void update(double dt) {
		if (!canFire) {
			timeSinceLastFire += dt;
		}
	}
};

#endif
