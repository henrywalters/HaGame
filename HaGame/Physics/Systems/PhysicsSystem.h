#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/ECS/Entity.h"
#include "../Components/RigidBody.h"

namespace hagame {
	namespace physics {
		class PhysicsSystem: public hagame::ecs::System {

			String getSystemName() {
				return "hagame::physics::PhysicsSystem";
			}

			void onSystemAfterUpdate(double dt) {
				forEach<RigidBody>([this, dt](RigidBody* rb, hagame::ecs::Entity* entity) {
					rb->applyGlobalGravity();
					rb->update(dt);
					entity->transform.move(rb->vel * dt);
				});
			}
		};
	}
}

#endif
