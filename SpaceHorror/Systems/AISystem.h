#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "../Components/Platformer.h"
#include "../Components/AI/Walker.h"

class AISystem : public hagame::ecs::System {
public:

	String getSystemName() { return "AISystem"; }

	void onSystemUpdate(double dt);
	void onSystemPhysicsUpdate(double dt);

};

#endif
