#ifndef PLATFORMER_SYSTEM_H
#define PLATFORMER_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "../Components/Platformer.h"
#include "../Components/PlayerController.h"

class PlatformerSystem : public hagame::ecs::System {
public:

	String getSystemName() { return "PlatformerSystem"; }

	void onSystemAfterUpdate(double dt);
	void onSystemPhysicsUpdate(double dt);

private:

	struct EntityHit {
		Ptr<Entity> entity;
		Vec3 normal;
		float t;
	};

	// Return the allowable velocity after collisions as well the directions collisions are occuring. Ordered by N,E,S,W
	void resolveCollisions(Ptr<hagame::ecs::Entity> entity, Vec3& velocity, std::array<bool, 4>& directions);
};

#endif
