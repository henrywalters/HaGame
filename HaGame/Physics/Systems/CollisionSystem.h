#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../../Core/Game.h"
#include "../../Core/ECS/System.h"
#include "../Components/Collider.h"
#include "../../Math/Functions.h"

namespace hagame {
	namespace physics {
		class CollisionSystem : public ecs::System {
		public:

			String getSystemName() {
				return "hagame::physics::CollisionSystem";
			}

			void onSystemStart() {
				forEach<Collider>([this](Collider* c, ecs::Entity* entity) {
					c->updateAABB(entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->position);
				});
			}

			void onSystemBeforeUpdate(double dt) {
				game->collisions.entityMap.clear();
				forEach<Collider>([this, dt](Collider* c, ecs::Entity* entity) {
					if (c->active) {
						game->collisions.entityMap.insert(entity->transform->position, entity);

						if (c->dynamic) {
							// TODO: add overloads for transform bounding box with AABB to prevent conversion between cube and AABB.
							c->updateAABB(entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->position);
						}

						if (c->display && c->shader) {
							c->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
							c->shader->setVec4("color", hagame::graphics::Color(54, 134, 255));
							hagame::graphics::drawCubeOutline(c->aabb.getCube(), hagame::graphics::Color::green(), c->shader);
						}
					}
				});
			}

			void onSystemAfterUpdate(double dt) {
				forEach<Collider>([this, dt](Collider* c, ecs::Entity* entity) {
					if (c->active) {
						if (c->display && c->shader) {
							c->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
							c->shader->setVec4("color", hagame::graphics::Color(54, 134, 255));
							hagame::graphics::drawCubeOutline(c->aabb.getCube(), hagame::graphics::Color::green(), c->shader);
						}
					}
				});
			}
		};
	}
}

#endif
