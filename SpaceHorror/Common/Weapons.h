#ifndef WEAPONS_H
#define WEAPONS_H

#include <optional>
#include <string>
#include <vector>
#include "../../HaGame/Graphics/Sprite.h"
#include "../../HaGame/Utils/File.h"
#include "../../HaGame/Utils/ConfigParser.h"

enum class BulletType {
	Raycast, // Use 0 in config
	Projectile, // Use 1 in config
};

struct Bullet {
	size_t id;

	// Required parameters
	std::string name;
	float damage;
	float spread;
	bool explodes;
	BulletType type;

	// Optional parameters depending on BulletType
	std::optional<float> speed;
	std::optional<float> blastRadius;
	std::optional<hagame::graphics::Sprite> projectileSprite;
};

struct Weapon {
	size_t id;

	// Required parameters
	std::string name;
	int bulletsPerShot;
	int clipCapacity;
	int totalCapacity;
	bool automatic;
	Bullet bullet;

	// Depending on automatic or not
	std::optional<float> fireRate;
	
};

std::vector<Bullet> parseBulletConfig(hagame::utils::File file);
std::vector<Weapon> parseWeaponConfig(hagame::utils::File file, std::vector<Bullet> bullets);

#endif
