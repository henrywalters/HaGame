#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/ECS/Entity.h"
#include "../Components/RigidBody.h"
#include "../../Core/Game.h"
#include "../../Math/OBB.h"

namespace hagame {
	namespace physics {
		class PhysicsSystem: public hagame::ecs::System {

			String getSystemName() {
				return "hagame::physics::PhysicsSystem";
			}

			void onSystemPhysicsBeforeUpdate(double dt) {
				forEach<RigidBody>([this, dt](RigidBody* rb, RawPtr<ecs::Entity> entity) {
					rb->clearForces();
					rb->applyGlobalGravity();
				});
			}

			void onSystemPhysicsAfterUpdate(double dt) {
				forEach<RigidBody>([this, dt](RigidBody* rb, RawPtr<ecs::Entity> entity) {
					rb->update(dt);
				});
			}
		};
	}
}

#endif
