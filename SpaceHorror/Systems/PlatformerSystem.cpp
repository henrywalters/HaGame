#include "PlatformerSystem.h"

void PlatformerSystem::onSystemUpdate(double dt) {
	forEach<Platformer>([this, dt](Platformer* p, Ptr<hagame::ecs::Entity> entity) {
		auto rigidbody = entity->getComponent<hagame::physics::RigidBody>();
		auto velocity = rigidbody->vel;
		velocity *= dt;

		Vec3 velX = velocity * Vec3::Right();
		Vec3 velY = velocity * Vec3::Top();
		
		std::array<bool, 4> collisionDirsX;
		std::array<bool, 4> collisionDirsY;

		// Resolve collisions to avoid entering walls
		resolveCollisions(entity, velX, collisionDirsX);
		if ((collisionDirsX[3] && velocity[0] < 0.0f) || (collisionDirsX[1] && velocity[0] > 0.0f)) {
			rigidbody->vel[0] = 0.0f;
		}

		resolveCollisions(entity, velY, collisionDirsY);
		if ((collisionDirsY[2] && velocity[1] < 0.0f) || (collisionDirsY[0] && velocity[1] > 0.0f)) {
			rigidbody->vel[1] = 0.0f;
		}

		// Apply drag forces in air or ground
		auto drag = collisionDirsY[2] ? p->groundDrag : p->airDrag;

		std::cout << Vec3::Right(velocity[0] * -drag * dt) << "\n";

		rigidbody->applyForce(Vec3::Right(velocity[0] * -drag));

		entity->transform->move(velX + velY);
	});
};


void PlatformerSystem::resolveCollisions(Ptr<hagame::ecs::Entity> entity, Vec3& velocity, std::array<bool, 4>& directions)
{

	if (velocity[0] == 0.0f && velocity[1] == 0.0f) return;

	auto collider = entity->getComponent<hagame::physics::Collider>();
	auto cube = collider->boundingCube.value();
	cube.pos += entity->getPos();
	auto ray = hagame::math::Ray(entity->getPos(), velocity);
	float velMag = velocity.magnitude();
	Cube groundCheck = Cube(cube.pos + velocity, Vec3(cube.size[0], cube.size[1] * 0.1f, 0));

	auto nCheck = Cube(cube.pos + velocity + Vec3::Top(cube.size[1] * 0.9f), Vec3(cube.size[0], cube.size[1] * 0.1f, 0));
	auto sCheck = Cube(cube.pos + velocity, Vec3(cube.size[0], cube.size[1] * 0.1f, 0));
	auto wCheck = Cube(cube.pos + velocity + Vec3::Top(cube.size[1] * 0.1f), Vec3(cube.size[0] * 0.1f, cube.size[1] * 0.8f, 0));
	auto eCheck = Cube(cube.pos + velocity + Vec3::Right(cube.size[0] * 0.9f) + Vec3::Top(cube.size[1] * 0.1f), Vec3(cube.size[0] * 0.1f, cube.size[1] * 0.8f, 0));

	std::array<hagame::math::AABB, 4> dirColliders = {
		hagame::math::AABB(nCheck),
		hagame::math::AABB(eCheck),
		hagame::math::AABB(sCheck),
		hagame::math::AABB(wCheck)
	};

	float t;

	Array<EntityHit> hits;

	for (int i = 0; i < 4; i++) {
		directions[i] = false;
	}

	scene->ecs.entities.forEach<hagame::physics::Collider>([&hits, &t, velMag, ray, entity, cube, dirColliders, &directions](hagame::physics::Collider* nCollider, Ptr<hagame::ecs::Entity> neighbor) {
		if (neighbor == entity) return;

		auto nCube = nCollider->boundingCube.value();
		nCube.pos += neighbor->getPos();

		auto nCubeExpanded = nCube;
		nCubeExpanded.pos -= (cube.size / 2.0f);
		nCubeExpanded.size += cube.size;


		auto hit = hagame::math::collisions::checkRayAgainstRect(ray, Rect(nCubeExpanded.pos, nCubeExpanded.size), t);

		if (hit.has_value() && t < 1.0f) {

			hits.push_back(EntityHit{ neighbor, hit.value().normal, t });

			for (int i = 0; i < 4; i++) {
				auto dirHit = hagame::math::collisions::checkAABBAgainstAABB(dirColliders[i], hagame::math::AABB(nCube));
				if (dirHit.has_value()) {
					directions[i] = true;
				}
			}

		}
	});

	std::sort(hits.begin(), hits.end(), [](const EntityHit a, const EntityHit b) {
		return a.t > b.t;
		});

	for (auto hit : hits) {
		velocity += hit.normal * Vec2(abs(velocity[0]), abs(velocity[1])) * (1 - hit.t);
	}

}
