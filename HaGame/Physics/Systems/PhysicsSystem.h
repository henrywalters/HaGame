#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/ECS/Entity.h"
#include "../Components/RigidBody.h"
#include "../../Core/Game.h"

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
					entity->transform->move(rb->vel * dt);
					auto collidingWith = game->collisions.queryAABB(entity->getComponent<Collider>()->aabb);

					//auto cellsToCheck = calcCellsThatIntersectLine(CHUNK_SIZE, entity->transform->position, rb->vel);

					//std::cout << "Cells to check: " << cellsToCheck.size() << std::endl;

					// std::cout << "Colliding with: " << collidingWith.size() << std::endl;
				});
			}
		};
	}
}

#endif
