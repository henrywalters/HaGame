#ifndef G_WEAPON_SYSTEM_H
#define G_WEAPON_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Components/Bullet.h"
#include "./../Components/Projectile.h"
#include "./../Components/Weapon.h"
#include "./../Components/Inventory.h"

struct BulletHitEvent {
	uint32_t targetId;
	Bullet bullet;
};

class WeaponSystem : public hagame::ecs::System {
public:

	hagame::Publisher<BulletHitEvent> hitPublisher;

	String getSystemName() {
		return "WeaponSystem";
	}

	void fire(Ptr<hagame::ecs::Entity> entity, Vec3 position, Vec3 direction, Ptr<Weapon> weapon) {

		if (!weapon->fire(true)) {
			return;
		}

		for (int i = 0; i < weapon->weapon.bulletsPerShot; i++) {
			if (weapon->weapon.type == WeaponType::Proj && weapon->weapon.bullet.has_value()) {
				auto bulletContainer = scene->addEntity();
				bulletContainer->addTag("bullet");
				bulletContainer->transform->setRotation(Quat(atan2(direction[1], direction[0]), Vec3::Face()));
				bulletContainer->transform->setPosition(position);
				auto bullet = bulletContainer->addComponent<Projectile>();
				bullet->ownerId = entity->id;
				auto bulletRenderer = bulletContainer->addComponent<hagame::graphics::SpriteRenderer>();
				bulletRenderer->shader = game->resources->getShaderProgram("sprite");
				bulletRenderer->sprite = std::make_shared<hagame::graphics::Sprite>(
					game->resources->getTexture(weapon->weapon.bullet.value().projectile.value().sprite),
					Rect(Vec2::Zero(), weapon->weapon.bullet.value().projectile.value().size)
					);
				bullet->bullet = weapon->weapon.bullet.value();

				bullet->direction = Quat(
					game->random.real<float>(
						-weapon->weapon.bullet.value().spread / 2.0f,
						weapon->weapon.bullet.value().spread / 2.0f
						),
					Vec3::Face()
				).rotatePoint(direction).resize<2>().normalized();

				auto bulletCollider = bulletContainer->addComponent<hagame::physics::Collider>();
				bulletCollider->boundingCube = Cube(Vec3::Zero(), weapon->weapon.bullet.value().projectile.value().size.resize<3>());
				bulletCollider->boundingSphere = hagame::physics::Collisions::getBoundingSphere(entity, bulletCollider);
				bulletCollider->boundingSphere.value().center = Vec3::Zero();
				bulletCollider->type = hagame::physics::ColliderType::SphereCollider;
				bulletCollider->shader = game->resources->getShaderProgram("color");
				// bulletCollider->ignoreTags.push_back("player");
				bulletCollider->ignoreEntities.push_back(entity->id);
			}
			else if (weapon->weapon.type == WeaponType::Raycast && weapon->weapon.bullet.has_value()) {

				auto adjustedDir = Quat(
					game->random.real<float>(
						-weapon->weapon.bullet.value().spread / 2.0f,
						weapon->weapon.bullet.value().spread / 2.0f
						),
					Vec3::Face()
				).rotatePoint(direction);

				float t;
				auto ray = hagame::math::Ray(position, adjustedDir * 1000000);

				auto raycastHit = game->collisions.raycast(entity, ray, t, { "bullet" });

				if (raycastHit.has_value()) {
					// Hit something!
					//hagame::graphics::drawRect(Rect(
					//	ray.getPointOnLine(t).resize<2>() - Vec2(2.5f),
					//	Vec2(5.0f)
					//), hagame::graphics::Color::green(), game->resources->getShaderProgram("color"));

					auto bulletHole = scene->addEntity();
					bulletHole->transform->setPosition(ray.getPointOnLine(t));
					bulletHole->transform->position[2] = 1.0f;
					auto holeR = bulletHole->addComponent<hagame::graphics::SpriteRenderer>();
					holeR->sprite = std::make_shared<hagame::graphics::Sprite>(game->resources->getTexture("bullethole"), Rect(Vec2::Zero(), Vec2(8.0f)));
					holeR->shader = game->resources->getShaderProgram("sprite");
				}
			}
		}

		
	}

	void onSystemUpdate(double dt) {

		forEach<Inventory>([this, dt](Inventory* inv) {
			inv->updateWeapons(dt);
		});

		forEach<Projectile>([this, dt](Projectile* bullet, Ptr<hagame::ecs::Entity> entity) {
			bullet->aliveFor += dt;
			float t;

			Vec3 d1 = Vec3({ bullet->direction[0], 0.0f, 0.0f });
			Vec3 d2 = Vec3({ 0.0f, bullet->direction[1], 0.0f });

			auto collider = entity->getComponent<hagame::physics::Collider>();

			if (collider != NULL) {
				if (game->collisions.checkCollisions(
					entity,
					entity->getComponent<hagame::physics::Collider>(),
					bullet->direction.resize<3>() * bullet->bullet.speed,
					dt,
					t
				)) {
					entity->transform->move(bullet->direction.resize<3>() * bullet->bullet.speed * dt * t);
					if (t == 0) {
						scene->removeEntity(entity);
					}
					
				}
				else {
					entity->transform->move(bullet->direction.resize<3>() * bullet->bullet.speed * dt);

					if (bullet->aliveFor >= 2.0) {
						scene->removeEntity(entity);
					}
				}
			}


		});
	}
};

#endif
