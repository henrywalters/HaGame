#ifndef PLATFORMER_SYSTEM_H
#define PLATFORMER_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "../Components/Platformer.h"
#include "../Components/PlayerController.h"
#include "../Components/MovingPlatform.h"

class PlatformerSystem : public hagame::ecs::System {
public:

	String getSystemName() { return "PlatformerSystem"; }

	void onSystemUpdate(double dt);
	void onSystemPhysicsUpdate(double dt);

private:

	struct EntityHit {
		RawPtr<Entity> entity;
		Vec3 normal;
		float t;
	};

	// Return the allowable velocity after collisions as well the directions collisions are occuring. Ordered by N,E,S,W
	void resolveCollisions(RawPtr<hagame::ecs::Entity> entity, Vec3& velocity, std::array<bool, 4>& directions);
};

#endif
