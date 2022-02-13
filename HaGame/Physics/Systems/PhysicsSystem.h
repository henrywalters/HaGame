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

			void onSystemBeforeUpdate(double dt) {
				forEach<RigidBody>([this, dt](RigidBody* rb, Ptr<ecs::Entity> entity) {
					rb->clearForces();

				});
			}

			void onSystemAfterUpdate(double dt) {
				forEach<RigidBody>([this, dt](RigidBody* rb, Ptr<ecs::Entity> entity) {
					rb->applyGlobalGravity();
					//std::cout << rb->netForce << "\n";
					rb->update(dt);

					

					game->resources->getShaderProgram("text")->use();
					game->resources->getShaderProgram("text")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));


					// entity->transform->move(rb->vel * dt);

				});
				
			}
		};
	}
}

#endif
