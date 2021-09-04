#ifndef G_WEAPON_H
#define G_WEAPON_H

#include "Bullet.h"
#include <optional>
#include <string>

enum WeaponType {
	Proj,
	Raycast,
	Melee,
};

struct WeaponDef {
	uint_fast8_t id;
	WeaponType type;
	std::optional<Bullet> bullet;
	int bulletsPerShot;
	int clipCapacity;
	int totalCapacity;
	float fireRate;
	bool automatic;
	std::string animation;
	Optional<String> shotSound;
};

struct Weapon {
	WeaponDef weapon;
	bool canFire = true;
	double timeSinceLastFire = 0.0;

	int clipAmmo = 0;
	int totalAmmo = 0;

	Weapon(WeaponDef _weapon) : weapon(_weapon) {}

	void update(double dt) {
		if (!canFire) {
			timeSinceLastFire += dt;

			if (timeSinceLastFire >= weapon.fireRate) {
				timeSinceLastFire = 0.0;
				canFire = true;
			}
		}
	}

	void addAmmo(int amount) {
		int canAdd = weapon.totalCapacity - totalAmmo;
		totalAmmo += clamp<int>(0, amount, canAdd);
	}

	bool reload() {
		int missingInClip = weapon.clipCapacity - clipAmmo;
		if (totalAmmo <= 0) {
			return false;
		}
		else if (totalAmmo < missingInClip) {
			int canAdd = missingInClip - totalAmmo;
			clipAmmo += canAdd;
			totalAmmo -= canAdd;
			return true;
		}
		else {
			clipAmmo += missingInClip;
			totalAmmo -= missingInClip;
			return true;
		}
	}

	bool fire(bool ignoreAmmo = false) {
		if (canFire && (ignoreAmmo || clipAmmo > 0)) {
			canFire = false;
			return true;
		}
		return false;
	}
};

constexpr uint_fast8_t HANDGUN{ 1 << 0 };
constexpr uint_fast8_t SHOTGUN{ 1 << 1 };
constexpr uint_fast8_t RIFLE{ 1 << 2 };
constexpr uint_fast8_t PLASMA_RIFLE{ 1 << 3 };
constexpr uint_fast8_t ROCKET_LAUNCHER{ 1 << 4 };

const WeaponDef HANDGUN_DEF{
	HANDGUN,
	WeaponType::Raycast,
	HANDGUN_BULLET,
	1,
	10,
	120,
	0.1f,
	false,
	"handgun",
	"handgun_shot"
};

const WeaponDef SHOTGUN_DEF{
	SHOTGUN,
	WeaponType::Raycast,
	SHOTGUN_PELLET,
	10,
	8,
	42,
	0.2f,
	false,
	"shotgun",
	"shotgun_shot"
};

const WeaponDef RIFLE_DEF{
	RIFLE,
	WeaponType::Raycast,
	RIFLE_BULLET,
	1,
	30,
	240,
	0.15f,
	true,
	"rifle",
	"rifle_shot"
};

const WeaponDef PLASMA_RIFLE_DEF{
	PLASMA_RIFLE,
	WeaponType::Proj,
	PLASMA_BLOB,
	1,
	30,
	120,
	0.25f,
	true,
	"rifle",
	"plasma_shot"
};

const WeaponDef ROCKET_LAUNCHER_DEF{
	ROCKET_LAUNCHER,
	WeaponType::Proj,
	ROCKET,
	1,
	1,
	8,
	2.0f,
	false,
	"rifle",
	std::nullopt
};

#endif
