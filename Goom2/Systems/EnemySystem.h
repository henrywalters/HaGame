#ifndef G_ENEMY_SYSTEM_H
#define G_ENEMY_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Components/Enemy.h"

class EnemySystem : public hagame::ecs::System {
public:
	String getSystemName() {
		return "EnemySystem";
	}


	void onSystemUpdate(double dt) {
		
		forEach<Enemy>([this, dt](Enemy* enemy, Ptr<hagame::ecs::Entity> entity) {
			enemy->weapon->update(dt);
			int rayCasts = 26;

			Ptr<hagame::ecs::Entity> spotted = NULL;

			auto targetPoint = entity->transform->position + entity->transform->top() * enemy->def.sightDistance;
			auto targetRay = hagame::math::Ray(entity->transform->position, targetPoint - entity->transform->position);

			auto targetRayPerp = entity->transform->right();
			
			auto sightSize = tan(enemy->def.sightAngle / 2) * enemy->def.sightDistance * 2;
			auto space = sightSize / rayCasts;

			for (int i = -rayCasts / 2; i <= rayCasts / 2; i++) {
				auto endPoint = targetPoint + (targetRayPerp * i * space);

				auto ray = hagame::math::Ray(entity->transform->position, endPoint - entity->transform->position);

				float t;
				Optional<Ptr<hagame::ecs::Entity>> entityInSight;
				entityInSight = game->collisions.raycast(entity, ray, t, { "bullet" });

				if (entityInSight.has_value()) {
					if (entityInSight.value()->hasTag("player")) {
						spotted = entityInSight.value();
						if (enemy->state == "SEARCH") {
							enemy->state = "TARGET";
						}
						
					}
				}
				else {
					t = 1.0f;
				}

				hagame::graphics::drawLine(hagame::graphics::Line(
					entity->transform->position,
					ray.getPointOnLine(t),
					hagame::graphics::Color(0.0f, 1.0f, 1.0f, 0.2f)
				), game->resources->getShaderProgram("color"));
				
			}

			auto angle = spotted != NULL ? entity->transform->top().angleBetween((spotted->transform->position - entity->transform->position)) : 0.0f;

			if (enemy->state == "SEARCH") {
				entity->transform->rotate(Quat(1.0f * dt, Vec3::Face()));
			}
			else if (enemy->state == "TARGET") {
				if (spotted == NULL) {
					enemy->state = "SEARCH";
				}
				else {
					
					if (angle > 0.01) {
						entity->transform->rotate(Quat(1.0f * dt, Vec3::Face()));
					}
					else if (angle < -0.01) {
						entity->transform->rotate(Quat(-1.0f * dt, Vec3::Face()));
					}
					else {
						enemy->state = "ATTACK";
					}
				}
			}
			else {
				if (spotted == NULL) {
					enemy->state = "SEARCH";
				}
				else {
					if (abs(angle) >= 0.01) {
						enemy->state = "TARGET";
					}
					else {
						scene->getSystem<WeaponSystem>()->fire(entity, entity->transform->position, targetRay.direction, enemy->weapon);
					}
				}
				
			}
		});
	}
};

#endif