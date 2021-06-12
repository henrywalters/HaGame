#ifndef COLLISIONS
#define COLLISIONS
#include "../Utils/Aliases.h"
#include "../Core/Game.h"
#include "../Utils/SpatialMap.h"
#include "../Core/ECS/Entity.h"
#include "../Math/AABB.h"
#include "./Components/Collider.h"

namespace hagame {
	namespace physics {

		const Vec3 CHUNK_SIZE = Vec3(10.0f);

		bool isColliding(Rect r1, Rect r2);

		class Collisions {
		public:
			utils::SpatialMap<ecs::Entity, float> entityMap;

			Collisions() : entityMap(utils::SpatialMap<ecs::Entity, float>(CHUNK_SIZE)) {}

			Array<ecs::Entity*> queryAABB(math::AABB aabb) {
				Array<ecs::Entity*> entities;
				std::cout << "Checking: " << entityMap.get(aabb.center).size() << std::endl;
				for (auto other : entityMap.get(aabb.center)) {
					auto otherCollider = other->getComponent<Collider>();
					if (otherCollider->aabb.isIntersecting(aabb)) {
						entities.push_back(other);
					}
				}
				return entities;
			}
		};
	}
}

#endif