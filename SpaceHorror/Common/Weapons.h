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

	// How much does running affect the spread of the bullet
	float spreadRunFactor;
	
	// Mean of the log normal spread
	float spreadMean;

	// Std deviation of the log normal spread
	float spreadStdDev;

	// Does the bullet explode on impact?
	bool explodes;


	BulletType type;

	// Optional parameters depending on BulletType
	std::optional<float> speed;
	std::optional<float> blastRadius;
	std::optional<hagame::graphics::Sprite> projectileSprite;
	std::optional<std::string> blastParticle;

	float getSpread(Vec3 velocity) {
		hagame::utils::Random random;
		float speed = velocity.magnitude();
		float r = sigmoid(speed) * spreadRunFactor;
		return random.normal(spreadMean, spreadStdDev * r) + r;
	}
};

struct BulletParticles {
	hagame::graphics::EmitterSettings hit;
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
	std::optional<std::string> blastParticle;

	// Depending on automatic or not
	std::optional<float> fireRate;
	
};

struct WeaponParticles {
	hagame::graphics::EmitterSettings shoot;
};

std::vector<Bullet> parseBulletConfig(hagame::utils::File* file);
std::vector<Weapon> parseWeaponConfig(hagame::utils::File* file, std::vector<Bullet> bullets);

#endif
