#ifndef ROTATION_MOVEMENT_H
#define ROTATION_MOVEMENT_H

#include "../../Utils/Aliases.h"

namespace hagame {
	namespace physics {
		
		// RotationMovement will automatically rotate the entity based on the axisOfRotation and speed.
		struct RotationMovement {
			Vec3 axis = Vec3::Top();
			float speed = 1.0f;
		};
	}
}

#endif
