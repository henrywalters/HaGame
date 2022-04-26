#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../../Utils/Aliases.h"

namespace hagame {
	namespace physics {
		struct RigidBody {

			Vec3 netForce = Vec3::Zero();
			Vec3 accel = Vec3::Zero();
			Vec3 vel = Vec3::Zero();

			Vec3Bool fixAxis = Vec3Bool(false);
			float mass = 1.0f;
			Vec3 forceDueToGravity = Vec3(0, -9.8f, 0);

			RigidBody() {}

			void clearForces() {
				netForce.zero();
			}

			void applyForce(Vec3 force) {
				netForce += force;
			}

			void applyGlobalGravity() {
				applyForce(forceDueToGravity * mass);
			}

			void update(double dt) {
				accel = netForce / mass;
				accel.update([this](int idx, float accel) { return fixAxis[idx] ? 0.0f : accel; });
				vel += accel * dt;
				// clearForces();
			}
		};
	}
}

#endif