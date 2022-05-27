#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Components/Health.h"
#include "./../Components/HealthBarRenderer.h"

class HealthSystem : public hagame::ecs::System {
public:
	String getSystemName() { return "HealthSystem"; }

	void onSystemUpdate(double dt) {
		forEach<Health>([this](Health* health, RawPtr<hagame::ecs::Entity> entity) {
			auto renderer = entity->getComponent<HealthBarRenderer>();
			if (renderer != nullptr) {
				renderer->setHealth(health->health);
			}
			if (!health->isAlive()) {
				scene->removeEntity(entity);
			}
		});
	}
};

#endif
