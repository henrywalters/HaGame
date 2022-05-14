#ifndef WEAPON_SYSTEM_H
#define WEAPON_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Common/Weapons.h"
#include "./../Components/WeaponController.h"
#include "./../Components/BulletHole.h"

class WeaponSystem : public hagame::ecs::System {
public:
	
	String getSystemName() { return "WeaponSystem"; }

	void fire(RawPtr<hagame::ecs::Entity> shooter, Weapon weapon, Vec3 direction) {
		auto ray = hagame::math::Ray(shooter->getPos(), direction);
		float t;
		auto hit = game->collisions.raycast(ray, scene->ecs.entities, t, {}, { shooter->uuid });
		if (hit.has_value()) {

			if (weapon.bullet.blastParticle.has_value()) {
				auto bulletHole = scene->addEntity();
				bulletHole->setPos(hit.value().point);
				
				auto renderer = bulletHole->addComponent<ParticleEmitterRenderer>();
				renderer->emitter->settings = m_bulletParticles[weapon.bullet.id].hit;
				renderer->emitter->fire(game->secondsElapsed);
				bulletHole->addComponent<BulletHole>(game->secondsElapsed)->aliveFor = renderer->emitter->settings.maxLife;
				renderer->shader = game->resources->getShaderProgram("particle");
				renderer->emitter->settings.dir = hit.value().normal;
			}
		}
	}

	void onSystemInit() {

		auto bulletConfig = hagame::utils::File(CONFIG_DIR, "Bullets.conf");
		auto weaponConfig = hagame::utils::File(CONFIG_DIR, "Weapons.conf");

		m_bullets = parseBulletConfig(&bulletConfig);
		m_weapons = parseWeaponConfig(&weaponConfig, m_bullets);

		for (auto bullet : m_bullets) {
			BulletParticles particles;
			if (bullet.blastParticle.has_value()) {
				auto file = hagame::utils::File(CONFIG_DIR + "/Particles", bullet.blastParticle.value());
				auto config = hagame::utils::ConfigFile(&file);
				particles.hit.loadFromConfig(config);
			}
			m_bulletParticles.insert(std::make_pair(bullet.id, particles));
		}

		for (auto weapon : m_weapons) {
			WeaponParticles particles;
			if (weapon.blastParticle.has_value()) {
				auto file = hagame::utils::File(CONFIG_DIR + "/Particles", weapon.blastParticle.value());
				auto config = hagame::utils::ConfigFile(&file);
				particles.shoot.loadFromConfig(config);
			}
			m_weaponParticles.insert(std::make_pair(weapon.id, particles));
		}
	}

	Weapon getWeaponByName(String name) {
		int index = getIndex<Weapon, String>(m_weapons, name, [](Weapon wep) { return wep.name; });
		if (index != -1) {
			return m_weapons[index];
		}
	}

	void setMousePos(Vec2 mousePos) { m_mousePos = mousePos; }

	void onSystemBeforeUpdate(double dt) {

		forEach<BulletHole>([this](RawPtr<BulletHole> hole, RawPtr<Entity> entity) {
			if (game->secondsElapsed - hole->createdAt >= hole->aliveFor) {
				// std::cout << "REMOVING BULLET\n";
				scene->removeEntity(entity);
			}
		});
	}

	void onSystemUpdate(double dt) {
		forEach<WeaponController>([this, dt](WeaponController * controller, RawPtr<hagame::ecs::Entity> entity) {

			auto weaponController = entity->getComponent<WeaponController>();
			auto rigidbody = entity->getComponent<hagame::physics::RigidBody>();
			auto playerController = entity->getComponent<PlayerController>();

			controller->update(dt);

			float lookAngle = 0.0f;

			if (controller->playerControlled) {
				bool firing = false;
				if (controller->getWeapon().automatic) {
					firing = game->input.player(0).rTrigger;
				}
				else {
					firing = game->input.player(0).rTriggerPressed;
				}

				if (firing && weaponController->fire(true)) {
					auto mouseDelta = m_mousePos - entity->getPos();
					auto deltaAngle = atan2(mouseDelta[1], mouseDelta[0]);
					std::cout << firing << "\n";

					auto emitter = entity->getComponent<hagame::graphics::ParticleEmitterRenderer>();

					if (weaponController->getWeapon().blastParticle.has_value() && emitter != nullptr) {
						emitter->emitter->settings = m_weaponParticles[weaponController->getWeapon().id].shoot;
						emitter->emitter->settings.dir = mouseDelta.normalized() * 10;
						emitter->emitter->fire(game->secondsElapsed);
					}

					for (int i = 0; i < weaponController->getWeapon().bulletsPerShot; i++) {
						auto spread = weaponController->getWeapon().bullet.getSpread(rigidbody->vel);
						Vec3 dir = Vec3(10 * cos(deltaAngle + DEG_TO_RAD * spread), 10 * sin(deltaAngle + DEG_TO_RAD * spread));
						fire(entity, weaponController->getWeapon(), dir);
					}
				}
			}

			if (!game->input.usingGamepad(0)) {
				auto crosshairDims = weaponController->getCrosshair(rigidbody->vel) * 0.1;
				drawCrosshairs(m_mousePos, crosshairDims.lower, crosshairDims.upper, Color::green(), 0.01);
				playerController->lookingAt = m_mousePos;
			}
			else {
				auto looking = game->input.player(0).rAxis;
				if (abs(looking[1]) > 0.1f) {
					lookAngle += looking[1] * dt;
				}

				if (looking[0] > 0.9f) {
					lookAngle = 0;
				}

				if (looking[0] < -0.9f) {
					lookAngle = PI;
				}

				playerController->lookingAt = entity->getPos() + Quat(lookAngle, Vec3::Face()).rotatePoint(Vec3::Right());
			}
		});

	}

private:
	const String CONFIG_DIR = "../../../SpaceHorror/Config";

	Vec2 m_mousePos;

	std::vector<Bullet> m_bullets;
	std::vector<Weapon> m_weapons;

	Map<size_t, BulletParticles> m_bulletParticles;
	Map<size_t, WeaponParticles> m_weaponParticles;
};

#endif
