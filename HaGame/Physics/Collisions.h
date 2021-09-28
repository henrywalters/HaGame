#ifndef COLLISIONS
#define COLLISIONS
#include "../Utils/Aliases.h"
#include "../Core/Game.h"
#include "../Utils/SpatialMap.h"
#include "../Core/ECS/Entity.h"
#include "../Math/AABB.h"
#include "../Math/Ray.h"
#include "../Math/Plane.h"
#include "./Components/Collider.h"
#include "./../Core/Publisher.h"
#include <tuple>
#include <optional>

namespace hagame {
	namespace physics {

		struct CollisionEvent {
			Ptr<ecs::Entity> collider, target;
		};

		// Prime number to help LCD
		const Vec3 CHUNK_SIZE = Vec3(10000.0f);

		bool isColliding(Rect r1, Rect r2);

		class Collisions {
		public:

			// Publisher<CollisionEvent> events;

			utils::SpatialMap<ecs::Entity, float> entityMap;

			Collisions() : entityMap(utils::SpatialMap<ecs::Entity, float>(CHUNK_SIZE)) {}

			/*Array<Ptr<ecs::Entity>> queryAABB(math::AABB aabb) {
				Array<Ptr<ecs::Entity>> entities;
				std::cout << "Checking: " << entityMap.get(aabb.center).size() << std::endl;
				for (auto other : entityMap.get(aabb.center)) {
					auto otherCollider = other->getComponent<Collider>();
					if (otherCollider->aabb.isIntersecting(aabb)) {
						entities.push_back(other);
					}
				}
				return entities;
			}
			*/
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

			Optional<Ptr<ecs::Entity>> raycast(Ptr<ecs::Entity> origin, math::Ray ray, float& t, Array<String> ignoreTags = {}) {
				Optional<Ptr<ecs::Entity>> entity = std::nullopt;
				float currT;
				for (auto& [key, neighborhood] : entityMap.map) {
					for (auto neighbor : neighborhood) {

						if (neighbor->id == origin->id || (ignoreTags.size() > 0 && neighbor->hasTag(ignoreTags)))
							continue;

						auto nCollider = neighbor->getComponent<Collider>();

						if (nCollider->type == ColliderType::BoxCollider) {
							auto cube = nCollider->boundingCube.value();
							cube.pos += neighbor->transform->getPosition();
							if (ray.checkCube(cube, currT) && (currT < t || !entity.has_value())) {
								t = currT;
								entity = neighbor;
							}
						}
						else if (nCollider->type == ColliderType::SphereCollider) {
							auto sphere = nCollider->boundingSphere.value();
							sphere.center += neighbor->transform->getPosition();
							if (ray.checkSphere(sphere, currT) && (currT < t || !entity.has_value())) {
								t = currT;
								entity = neighbor;
							}
						}
					}
				}
				
				
				return entity;
			}

			Optional<Ptr<ecs::Entity>> checkCollisions(Ptr<ecs::Entity> entity, Collider* collider, Vec3 velocity, double dt, float &t) {
				if (collider != NULL && collider->dynamic && velocity.magnitude() > 0) {
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

					// collider->shader->use();

					if (collider->display) {
						hagame::graphics::drawLine(hagame::graphics::Line(rays[1].origin, rays[1].origin + rays[1].direction, hagame::graphics::Color::blue()), collider->shader);
						hagame::graphics::drawLine(hagame::graphics::Line(rays[0].origin, rays[0].origin + rays[0].direction, hagame::graphics::Color::blue()), collider->shader);
						hagame::graphics::drawLine(hagame::graphics::Line(rays[2].origin, rays[2].origin + rays[2].direction, hagame::graphics::Color::blue()), collider->shader);
					}

					bool collided = false;
					float tMin;
					Ptr<ecs::Entity> collidedWith;
					float collisionT;
					//for (auto& [key, neighborhood] : entityMap.map)
					//{
					for (auto neighbor : entityMap.get(entity->transform->getPosition() + velocity * dt)) {
						//for (auto neighbor : neighborhood) {
							if (neighbor->id == entity->id || hasElement(collider->ignoreEntities, neighbor->id) || neighbor->hasTag(collider->ignoreTags))
								continue;

							auto nCollider = neighbor->getComponent<Collider>();

							if (nCollider->type == ColliderType::BoxCollider) {
								auto cube = nCollider->boundingCube.value();
								cube.pos += neighbor->transform->getPosition();

								for (int i = 0; i < 3; i++) {
									if (rays[i].checkCube(cube, collisionT)) {
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
									if (rays[i].checkSphere(sphere, collisionT)) {
										if (collisionT < tMin || !collided) {
											tMin = collisionT;
											collidedWith = neighbor;
											collided = true;
										}
									}
								}
							}
						//}
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