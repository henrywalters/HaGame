#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/ECS/Entity.h"
#include "../Components/RigidBody.h"
#include "../Components/SimpleMovement.h"
#include "../Components/RotationMovement.h"

namespace hagame {
	namespace physics {
		class MovementSystem : public hagame::ecs::System {

			String getSystemName() {
				return "hagame::physics::MovementSystem";
			}

			void handleSimpleMovement(double dt) {
				forEach<SimpleMovement>([this, dt](SimpleMovement* sm, hagame::ecs::Entity* entity) {
					auto rb = entity->getComponent<RigidBody>();
					if (rb == NULL) {
						throw new std::exception("Simple Movement system requires RigidBody component");
					}

					auto params = sm->inputFn();

					Vec3 forwardDir = entity->transform.face();
					Vec3 strafeDir = entity->transform.right();

					Vec3 forwardMove = forwardDir * params.movement[1];
					Vec3 strafeMove = strafeDir * params.movement[0];

					sm->isRunning = params.running;

					rb->applyForce(forwardMove * dt * (sm->isRunning ? sm->runForce : sm->walkForce));
					rb->applyForce(strafeMove * dt * sm->strafeForce);

					if (rb->vel.magnitude() > EPSILON) {
						Vec3 velNormal = rb->vel.normalized();

						rb->applyForce(velNormal * -1 * sm->frictionForce * dt);
						rb->applyForce(velNormal * -1 * sm->dragForce * rb->vel.magnitude() * dt);
					}
				});
			}

			void handleRotationMovement(double dt) {
				forEach<RotationMovement>([this, dt](RotationMovement* rm, hagame::ecs::Entity* entity) {
					entity->transform.rotate(Quat(rm->speed * dt, rm->axis));
				});
			}

			void onSystemUpdate(double dt) {
				handleSimpleMovement(dt);
				handleRotationMovement(dt);
			}
		};
	}
}

#endif
