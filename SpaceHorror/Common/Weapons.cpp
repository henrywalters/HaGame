#include "Weapons.h"

std::vector<Bullet> parseBulletConfig(hagame::utils::File* file)
{
	std::vector<Bullet> bullets;
	auto config = hagame::utils::ConfigFile(file);
	int index = 0;
	for (auto section : config.getSections()) {
		Bullet bullet;
		bullet.id = ++index;
		bullet.name = section;
		bullet.damage = config.getFloat(section, "damage");
		bullet.spreadRunFactor = config.getFloat(section, "spread_run_factor");
		bullet.spreadMean = config.getFloat(section, "spread_mean");
		bullet.spreadStdDev = config.getFloat(section, "spread_stddev");
		bullet.explodes = config.getBool(section, "explodes");
		bullet.type = (BulletType)config.getInt(section, "type");

		if (bullet.type == BulletType::Projectile) {
			bullet.speed = config.getFloat(section, "speed");
		}

		if (bullet.explodes) {
			bullet.blastRadius = config.getFloat(section, "blastRadius");
		}
		bullets.push_back(bullet);
	}
	return bullets;
}

std::vector<Weapon> parseWeaponConfig(hagame::utils::File* file, std::vector<Bullet> bullets)
{
	std::vector<Weapon> weapons;
	int index = 0;
	auto config = hagame::utils::ConfigFile(file);
	for (auto section : config.getSections()) {
		Weapon weapon;
		weapon.id = ++index;
		weapon.name = section;
		weapon.bulletsPerShot = config.getInt(section, "bulletsPerShot");
		weapon.clipCapacity = config.getInt(section, "clipCapacity");
		weapon.totalCapacity = config.getInt(section, "totalCapacity");
		weapon.automatic = config.getBool(section, "automatic");
		
		if (weapon.automatic) {
			weapon.fireRate = config.getFloat(section, "fireRate");
		}

		auto bulletName = config.getRaw(section, "bullet");
		bool found = false;

		for (auto bullet : bullets) {
			if (bulletName == bullet.name) {
				found = true;
				weapon.bullet = bullet;
			}
		}

		if (!found) {
			std::cout << "Bullet: " << bulletName << " not found in config file.\n";
			throw new std::exception("Failed to load weapons");
		}

		weapons.push_back(weapon);
	}
	return weapons;
}
