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

			void onSystemUpdate(double dt) {
				/*forEach<RigidBody>([this, dt](RigidBody* rb, Ptr<ecs::Entity> entity) {
					rb->applyGlobalGravity();
					rb->update(dt);

					//auto aabb = hagame::math::AABB(entity->getComponent<Collider>()->boundingCube);
					//aabb.center = entity->getComponent<Collider>()->aabb.center;
					auto obb = aabb.getOBB();
					obb.orientation = Mat4::Rotation(entity->transform->rotation);

					game->resources->getShaderProgram("text")->use();
					game->resources->getShaderProgram("text")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));

					hagame::graphics::drawText(
						game->resources->getShaderProgram("text"),
						game->resources->getFont("roboto"),
						"Player OBB: " + obb.toString(),
						hagame::graphics::Color::green(),
						Vec3({ 15, game->window->size[1] - 50, 0 }),
						0.5f
					);
					
					for (int i = 0; i < 3; i++) {
						aabb.center += rb->vel[i] * dt;
						//auto aabbCollisions = game->collisions.queryAABB(aabb);
						for (auto coll : aabbCollisions) {
							if (coll->id != entity->id) {
								rb->vel[i] = 0;
								std::cout << "Checking OBB intersection\n";
								//auto otherObb = coll->getComponent<Collider>()->aabb.getOBB();
								otherObb.orientation = Mat4::Rotation(coll->transform->rotation);
								if (obb.isIntersecting(otherObb)) {
									rb->vel[i] = 0;
								}
								
							}
						}

						aabb.center -= rb->vel[i] * dt;
					}

					entity->transform->move(rb->vel * dt);
					//auto collidingWith = game->collisions.queryAABB(entity->getComponent<Collider>()->aabb);

					//auto cellsToCheck = calcCellsThatIntersectLine(CHUNK_SIZE, entity->transform->position, rb->vel);

					//std::cout << "Cells to check: " << cellsToCheck.size() << std::endl;
				});
				*/
			}
		};
	}
}

#endif
