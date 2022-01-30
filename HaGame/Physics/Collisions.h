#ifndef COLLISIONS
#define COLLISIONS
#include "../Utils/Aliases.h"
#include "../Core/Game.h"
#include "../Utils/SpatialMap.h"
#include "../Core/ECS/Entity.h"
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

			utils::SpatialMap<ecs::Entity, float> entityMap;

			Collisions() : entityMap(utils::SpatialMap<ecs::Entity, float>(Vec3(10.0f))) {}

			static Sphere getBoundingSphere(Ptr<ecs::Entity> entity, Collider* collider) {
				if (collider->type == hagame::physics::ColliderType::SphereCollider) {
					if (!collider->boundingSphere.has_value()) {
						throw new std::exception("SphereCollider must have boundingSphere defined");
					}
					auto bs = collider->boundingSphere.value();
					return Sphere(entity->transform->getPosition() + bs.center, bs.radius);
				}
				else if (collider->type == hagame::physics::ColliderType::BoxCollider) {
					if (!collider->boundingCube.has_value()) {
						throw new std::exception("BoxCollider must have boundingSphere defined");
					}
					auto bc = collider->boundingCube.value();
					auto radius = (max(bc.size[0], max(bc.size[1], bc.size[2])) / 2.0f) * SQRT_2;
					return Sphere(entity->transform->getPosition(), radius);
				}
				else {
					throw new std::exception("getBoundingSphere does not have support for this collider type");
				}
			}

			Optional<Hit> raycast(Ptr<ecs::Entity> origin, math::Ray ray, float& t, Array<String> ignoreTags = {}) {
				//Optional<Ptr<ecs::Entity>> entity = std::nullopt;
				Optional<Hit> hit;
				float currT;
				for (auto& [key, neighborhood] : entityMap.map) {
					for (auto neighbor : neighborhood) {
						if (neighbor->id == origin->id || (ignoreTags.size() > 0 && neighbor->hasTag(ignoreTags)))
							continue;

						auto nCollider = neighbor->getComponent<Collider>();

						if (nCollider->type == ColliderType::BoxCollider) {
							auto cube = nCollider->boundingCube.value();
							cube.pos += neighbor->transform->getPosition();
							//auto rayHit = ray.checkCube(cube, currT);
							auto rayHit = hagame::math::collisions::checkRayAgainstCube(ray, cube, currT);
							if (rayHit.has_value() && (currT < t || !hit.has_value())) {
								t = currT;
								hit = Hit{ neighbor, rayHit.value().position, rayHit.value().normal };
							}
						}
						else if (nCollider->type == ColliderType::SphereCollider) {
							auto sphere = nCollider->boundingSphere.value();
							sphere.center += neighbor->transform->getPosition();
							auto rayHit = hagame::math::collisions::checkRayAgainstSphere(ray, sphere, currT);
							if (rayHit.has_value() && (currT < t || !hit.has_value())) {
								t = currT;
								hit = Hit{ neighbor, rayHit.value().position, rayHit.value().normal };
							}
						}
					}
				}
				
				
				return hit;
			}

			Optional<Ptr<ecs::Entity>> checkCollisions(Ptr<ecs::Entity> entity, Collider* collider, Vec3 velocity, double dt, float &t) {
				if (collider != NULL && collider->dynamic) {
					auto bs = getBoundingSphere(entity, collider);
					auto velNorm = velocity.normalized();
					// math::Ray velRay = math::Ray(bs.center + velNorm * bs.radius, velocity * dt);
					auto velPerp = cross(velNorm, Vec3::Face()) * 100.0f;
					auto velPerpNorm = velPerp.normalized();

					auto origin = bs.center + velNorm * bs.radius;
					auto dir = velocity * dt;

					math::Ray rays[3] = {
						math::Ray(origin, dir),
						math::Ray(origin + velPerpNorm * bs.radius, dir),
						math::Ray(origin - velPerpNorm * bs.radius, dir)
					};

					bool collided = false;
					float tMin;
					Ptr<ecs::Entity> collidedWith;
					float collisionT;

					for (auto neighbor : entityMap.get(entity->transform->getPosition() + velocity * dt)) {
						if (neighbor->id == entity->id || hasElement(collider->ignoreEntities, neighbor->id) || neighbor->hasTag(collider->ignoreTags))
							continue;

						auto nCollider = neighbor->getComponent<Collider>();

						if (nCollider->type == ColliderType::BoxCollider) {
							auto cube = nCollider->boundingCube.value();
							cube.pos += neighbor->transform->getPosition();

							for (int i = 0; i < 3; i++) {
								auto rayHit = hagame::math::collisions::checkRayAgainstCube(rays[i], cube, collisionT);
								if (rayHit.has_value()) {
									// TODO: add fine grain collision check here but it seems to work great.
									if (collisionT < tMin || !collided) {
										tMin = collisionT;
										collidedWith = neighbor;
										collided = true;
									}
								}
							}
						}
						else if (nCollider->type == ColliderType::SphereCollider) {
							auto sphere = nCollider->boundingSphere.value();
							sphere.center += neighbor->transform->getPosition();

							for (int i = 0; i < 3; i++) {
								auto rayHit = hagame::math::collisions::checkRayAgainstSphere(rays[i], sphere, collisionT);
								if (rayHit.has_value()) {
									if (collisionT < tMin || !collided) {
										tMin = collisionT;
										collidedWith = neighbor;
										collided = true;
									}
								}
							}
						}
					}
					

					if (collided) {
						t = tMin;
						CollisionEvent e;
						e.collider = entity;
						e.target = collidedWith;
						// events.emit(e);
						return collidedWith;
					}
				}

				return std::nullopt;
			}
		};
	}
}

#endif