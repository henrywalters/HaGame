#ifndef COLLISIONS
#define COLLISIONS
#include "../Utils/Aliases.h"
#include "../Core/Game.h"
#include "../Utils/SpatialMap.h"
#include "../Core/ECS/Entity.h"
#include "../Core/ECS/EntityManager.h"
#include "../Math/AABB.h"
#include "../Math/Ray.h"
#include "../Math/Plane.h"
#include "../Math/Collisions.h"
#include "./Components/Collider.h"
#include "./../Core/Publisher.h"
#include <tuple>
#include <optional>

namespace hagame {
	namespace physics {

		struct CollisionEvent {
			Ptr<ecs::Entity> collider, target;
		};

		struct Hit {
			Ptr<ecs::Entity> entity;
			Vec3 point;
			Vec3 normal;
		};

		bool isColliding(Rect r1, Rect r2);

		class Collisions {
		public:

			// Publisher<CollisionEvent> events;

			[[deprecated("This implementation specific version of a spatial map is deprecated")]]
			utils::SpatialMap<ecs::Entity, float> entityMap;

			Collisions() : entityMap(utils::SpatialMap<ecs::Entity, float>(Vec3(10.0f))) {}

			static Sphere getBoundingSphere(Ptr<ecs::Entity> entity, Collider* collider);

			[[deprecated("This version of raycast requires the deprecated implementation specific entityMap")]]
			Optional<Hit> raycast(Ptr<ecs::Entity> origin, math::Ray ray, float& t, Array<String> ignoreTags = {});

			// Calculate the first entity that a ray collides with at time t. Optionally ignore a list of tags or entities.
			Optional<Hit> raycast(math::Ray ray, ecs::EntityManager& entities, float& t, Array<String> ignoreTags = {}, Array<uint64_t> ignoreEntities = {});

			[[deprecated("This version of checkCollisions requires the deprecated implementation specific entityMap")]]
			Optional<Ptr<ecs::Entity>> checkCollisions(Ptr<ecs::Entity> entity, Collider* collider, Vec3 velocity, double dt, float& t);

			Optional<hagame::math::collisions::Hit> checkCollision(BoundingVolume* check, BoundingVolume* against, Vec3 velocity, double dt, float& t);
			Optional<hagame::math::collisions::Hit> checkCollision(BoundingVolume* check, Vec3 checkPos, BoundingVolume* against, Vec3 againstPos);

		private:

			hagame::physics::Hit hit2hit(hagame::math::collisions::Hit hit, Ptr<hagame::ecs::Entity> entity);
		};
	}
}

#endif