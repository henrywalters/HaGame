#ifndef GRAVITY_SYSTEM
#define GRAVITY_SYSTEM

#include "./../../HaGame/HaGame.h"
#include "./../Components/GravityZone.h"

class GravitySystem : public hagame::ecs::System {
public:

	String getSystemName() { return "GravitySystem"; }

	void onSystemPhysicsUpdate(double dt) {
		forEach<GravityZone>([dt](GravityZone* zone, RawPtr<hagame::ecs::Entity> entity) {
				
		});
	}
};

#endif