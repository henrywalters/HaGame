#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../../Core/Game.h"
#include "../../Core/ECS/System.h"
#include "../Components/Collider.h"
#include "./../Collisions.h"
#include "../../Math/Functions.h"
#include "./PartitionSystem.h"

namespace hagame {
	namespace physics {
		class CollisionSystem : public ecs::System {
		public:

			Ptr<PartitionSystem> partitions;

			String getSystemName() {
				return "hagame::physics::CollisionSystem";
			}

			void onSystemStart() {
				//forEach<Collider>([this](Collider* c, Ptr<ecs::Entity> entity) {
				//	c->updateAABB(entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->getPosition());
				//});
				partitions = scene->getSystem<PartitionSystem>();
			}

			void onSystemBeforeUpdate(double dt) {
				game->collisions.entityMap.clear();
				forEach<Collider>([this, dt](Collider* c, Ptr<ecs::Entity> entity) {
					if (c->active) {
						game->collisions.entityMap.insert(entity->transform->getPosition(), entity);
					}
				});
			}

			void onSystemAfterUpdate(double dt) {
				forEach<Collider>([this, dt](Collider* c, Ptr<ecs::Entity> entity) {
					if (c->active) {
						// auto neighbors = 
					}
				});
			}
		};
	}
}

#endif
