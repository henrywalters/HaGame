#ifndef SIMPLE_MOVEMENT_H
#define SIMPLE_MOVEMENT_H

#include "../../Utils/Aliases.h"
#include "../../Core/Transform.h"
#include "../../Input/Device.h"

namespace hagame {
	namespace physics {

		struct SimpleMovementParameters {
			Vec2 movement;
			bool running;
		};

		// The SimpleMovement component provides a force based movement system
		struct SimpleMovement {
			float strafeForce = 800.0f;
			float walkForce = 1000.0f;
			float runForce = 1200.0f;
			float frictionForce = 50.0f;
			float dragForce = 200.0f;
			float maxStrafeSpeed = 3.0f;
			float maxWalkSpeed = 2.0f;
			float maxRunSpeed = 5.5f;

			bool canRun = true;
			bool isRunning = false;

			std::function<SimpleMovementParameters()> inputFn;
		};
	}
}

#endif