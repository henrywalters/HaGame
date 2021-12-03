#ifndef PROJECTILE_SYSTEM_HPP
#define PROJECTILE_SYSTEM_HPP

#include "./../../HaGame/HaGame.h"

class Projectile {

	double aliveFor;

public:

	Vec3 velocity;
	Vec3 gravity;
	double maxAge = 1.0f;

	bool update(double dt) {
		aliveFor += dt;
		
		if (aliveFor >= maxAge)
			return false;

		velocity += gravity * dt;

		return true;
	}
};

class ProjectileSystem : public hagame::ecs::System {
public:

	String getSystemName() { return "Projectile System"; }

	void onSystemUpdate(double dt) {
		forEach<Projectile>([this, dt](Projectile* proj, Ptr<hagame::ecs::Entity> entity) {

			if (!proj->update(dt))
				scene->removeEntity(entity);
			else
				entity->transform->move(proj->velocity * dt);
		});
	}
};

#endif
