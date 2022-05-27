#include "Collisions.h"
#include "Collisions.h"
#include "Collisions.h"

bool hagame::physics::isColliding(Rect r1, Rect r2)
{
	return r1.contains(r2.pos) ||
		r1.contains(r2.pos + Vec2({ r2.size[0] })) ||
		r1.contains(r2.pos + Vec2({ 0.0f, r2.size[1] })) ||
		r1.contains(r2.pos + r2.size);
}

Sphere hagame::physics::Collisions::getBoundingSphere(RawPtr<hagame::ecs::Entity> entity, Collider* collider)
{
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

Optional<hagame::physics::Hit> hagame::physics::Collisions::raycast(RawPtr<hagame::ecs::Entity> origin, math::Ray ray, float& t, Array<String> ignoreTags)
{
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

Optional<hagame::physics::Hit> hagame::physics::Collisions::raycast(math::Ray ray, hagame::ecs::EntityManager& entities, float& t, Array<String> ignoreTags, Array<uint64_t> ignoreEntities)
{
	Optional<Hit> hit;
	float currT;
	entities.forEach([this, &hit, &currT, &t, ray](RawPtr<ecs::Entity> entity) {

		auto nCollider = entity->getComponent<Collider>();

		if (nCollider == NULL) return;

		if (nCollider->type == ColliderType::BoxCollider) {
			auto cube = nCollider->boundingCube.value();
			cube.pos += entity->transform->getPosition();
			//auto rayHit = ray.checkCube(cube, currT);
			auto rayHit = hagame::math::collisions::checkRayAgainstCube(ray, cube, currT);
			if (rayHit.has_value() && (!hit.has_value() || currT < t)) {
				t = currT;
				hit = Hit{ entity, rayHit.value().position, rayHit.value().normal };
			}
		}
		else if (nCollider->type == ColliderType::SphereCollider) {
			auto sphere = nCollider->boundingSphere.value();
			sphere.center += entity->transform->getPosition();
			auto rayHit = hagame::math::collisions::checkRayAgainstSphere(ray, sphere, currT);
			if (rayHit.has_value() && (!hit.has_value() || currT < t)) {
				t = currT;
				hit = Hit{ entity, rayHit.value().position, rayHit.value().normal };
			}
		}
	}, ignoreEntities, ignoreTags); 

	return hit;
}

Array<RawPtr<hagame::ecs::Entity>> hagame::physics::Collisions::raycastSweep2D(math::Ray ray, float angle, int raycasts, ecs::EntityManager& entities, Array<String> ignoreTags, Array<uint64_t> ignoreEntities)
{
	Array<RawPtr<hagame::ecs::Entity>> out;
	auto step = angle / (float)raycasts;
	auto mag = ray.direction.magnitude();
	auto startAngle = atan2f(ray.direction[1], ray.direction[0]) - angle * 0.5f;
	std::set<long> entityIds;
	float t;

	for (int i = 0; i <= raycasts; i++) {
		auto angle = startAngle + i * step;
		Vec3 dir = Vec3(cos(angle) * mag, sin(angle) * mag);
		math::Ray tmpRay(ray.origin, dir);
		auto hit = raycast(tmpRay, entities, t, ignoreTags, ignoreEntities);
		if (hit.has_value()) {
			if (entityIds.find(hit.value().entity->id) == entityIds.end()) {
				out.push_back(hit.value().entity);
				entityIds.insert(hit.value().entity->id);
			}
		}
	}

	return out;
}

Optional<RawPtr<hagame::ecs::Entity>> hagame::physics::Collisions::checkCollisions(RawPtr<ecs::Entity> entity, Collider* collider, Vec3 velocity, double dt, float& t)
{
	if (collider != NULL && collider->dynamic) {
		auto bs = getBoundingSphere(entity, collider);
		bs.radius += velocity.magnitude();

		hagame::graphics::drawCircle2D(bs.center, bs.radius, hagame::graphics::Color::green());

		auto velNorm = velocity.normalized();
		// math::Ray velRay = math::Ray(bs.center + velNorm * bs.radius, velocity * dt);
		auto velPerp = cross(velNorm, Vec3::Face()) * 100.0f;
		auto velPerpNorm = velPerp.normalized();

		auto origin = bs.center + velNorm * bs.radius;
		auto dir = velocity * dt * 1000;

		math::Ray rays[3] = {
			math::Ray(origin, dir),
			math::Ray(origin + velPerpNorm * bs.radius, dir),
			math::Ray(origin - velPerpNorm * bs.radius, dir)
		};

		bool collided = false;
		float tMin;
		RawPtr<ecs::Entity> collidedWith;
		float collisionT;

		for (auto neighbor : entityMap.get(entity->transform->getPosition() + velocity * dt)) {
			if (neighbor->id == entity->id || hasElement(collider->ignoreEntities, neighbor->id) || neighbor->hasTag(collider->ignoreTags))
				continue;

			auto nCollider = neighbor->getComponent<Collider>();

			if (nCollider->type == ColliderType::BoxCollider) {
				auto cube = nCollider->boundingCube.value();
				cube.pos += neighbor->transform->getPosition();

				hagame::graphics::drawCubeOutline(cube, hagame::graphics::Color::red());

				auto broadHit = hagame::math::collisions::checkSphereAgainstAABB(bs, hagame::math::AABB(cube));

				if (broadHit.has_value()) {
					if (collider->type == ColliderType::BoxCollider) {
						auto entityCube = collider->boundingCube.value();
						entityCube.pos += entity->transform->getPosition();
						auto fineHit = hagame::math::collisions::checkAABBAgainstAABB(hagame::math::AABB(entityCube), hagame::math::AABB(cube));

						if (fineHit.has_value()) {
							tMin = broadHit.value().depth;
							collidedWith = neighbor;
							collided = true;
						}
					}
				}

				/*for (int i = 0; i < 3; i++) {
					auto rayHit = hagame::math::collisions::checkRayAgainstCube(rays[i], cube, collisionT);
					if (rayHit.has_value()) {
						// TODO: add fine grain collision check here but it seems to work great.
						if (collisionT < tMin || !collided) {
							tMin = collisionT;
							collidedWith = neighbor;
							collided = true;
						}
					}
				}*/
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

Optional<hagame::math::collisions::Hit> hagame::physics::Collisions::checkCollision(BoundingVolume* check, BoundingVolume* against, Vec3 velocity, double dt, float& t)
{
	return std::nullopt;
}

Optional<hagame::math::collisions::Hit> hagame::physics::Collisions::checkCollision(BoundingVolume* _check, Vec3 checkPos, BoundingVolume* _against, Vec3 againstPos)
{

	BoundingVolume check = *_check;
	BoundingVolume against = *_against;

	if (check.getType() == BoundingVolumeType::Cube && against.getType() == BoundingVolumeType::Cube) {
		check.getCube().pos += checkPos;
		against.getCube().pos += againstPos;
		return hagame::math::collisions::checkAABBAgainstAABB(hagame::math::AABB(check.getCube()), hagame::math::AABB(against.getCube()));
	}

	if (check.getType() == BoundingVolumeType::Cube && against.getType() == BoundingVolumeType::Sphere) {
		check.getCube().pos += checkPos;
		against.getSphere().center += againstPos;
		return hagame::math::collisions::checkSphereAgainstAABB(check.getSphere(), hagame::math::AABB(against.getCube()));
	}

	if (check.getType() == BoundingVolumeType::Sphere && against.getType() == BoundingVolumeType::Sphere) {
		check.getSphere().center += checkPos;
		against.getSphere().center += againstPos;
		return hagame::math::collisions::checkSphereAgainstSphere(check.getSphere(), against.getSphere());
	}


	if (check.getType() == BoundingVolumeType::Sphere && against.getType() == BoundingVolumeType::Cube) {
		check.getSphere().center += checkPos;
		against.getCube().pos += againstPos;
		return hagame::math::collisions::checkSphereAgainstAABB(against.getSphere(), hagame::math::AABB(check.getCube()));
	}

	std::cout << "WARNING: no collision check was performed\n";

	return std::nullopt;
}

hagame::physics::Hit hagame::physics::Collisions::hit2hit(hagame::math::collisions::Hit hit, RawPtr<hagame::ecs::Entity> entity)
{
	hagame::physics::Hit pHit;
	pHit.normal = hit.normal;
	pHit.point = hit.position;
	pHit.entity = entity;
	return pHit;
}
