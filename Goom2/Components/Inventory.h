#ifndef G_INVENTORY_H
#define G_INVENTORY_H

#include <optional>
#include "./../../HaGame/Utils/Aliases.h"
#include "Weapon.h"

class Inventory {
	Map<uint_fast8_t, Ptr<Weapon>> weapons;
	uint_fast8_t weaponsMask;
	std::optional<uint_fast8_t> _activeWeapon = std::nullopt;

public:

	Inventory() {
		weapons = Map<uint_fast8_t, Ptr<Weapon>>();
		weapons.insert(std::make_pair(HANDGUN, std::make_shared<Weapon>(HANDGUN_DEF)));
		weapons.insert(std::make_pair(SHOTGUN, std::make_shared<Weapon>(SHOTGUN_DEF)));
		weapons.insert(std::make_pair(RIFLE, std::make_shared<Weapon>(RIFLE_DEF)));
		weapons.insert(std::make_pair(PLASMA_RIFLE, std::make_shared<Weapon>(PLASMA_RIFLE_DEF)));
		weapons.insert(std::make_pair(ROCKET_LAUNCHER, std::make_shared<Weapon>(ROCKET_LAUNCHER_DEF)));
		
		weaponsMask = uint_fast8_t{ 0 };
	}

	void pickupWeapon(uint_fast8_t weapon) {
		weaponsMask |= weapon;
	}

	void dropWeapon(uint_fast8_t weapon) {
		weaponsMask &= ~weapon;
	}

	bool hasWeapon(uint_fast8_t weapon) {
		return static_cast<bool>(weaponsMask & weapon);
	}

	void pickupAmmo(uint_fast8_t weapon, int amount) {
		weapons[weapon]->addAmmo(amount);
	}

	void setWeapon(uint_fast8_t weapon) {
		if (hasWeapon(weapon)) {
			std::cout << "Set weapon to " << weapon << "\n";
			_activeWeapon = weapon;
		}
	}

	void updateWeapons(double dt) {
		for (auto& [id, weapon] : weapons) {
			weapon->update(dt);
		}
	}

	std::optional<Ptr<Weapon>> activeWeapon() {
		return _activeWeapon.has_value() ? std::optional<Ptr<Weapon>>(weapons[_activeWeapon.value()]) : std::nullopt;
	}
};

#endif
