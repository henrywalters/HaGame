#ifndef WALKER_H
#define WALKER_H

#include "./../../../HaGame/HaGame.h"
#include "./AI.h"

using namespace hagame::ecs;
using namespace hagame::physics;
using namespace hagame::graphics;

struct Walker : public AI {

	bool init = false;
	Ptr<hagame::utils::Promise<long, String, false>> turnAroundDelay;

	float force = 30.0f;
	float sightDistance = 5.0f;
	int meanDelay = 7.5;

	void idle(Vec3 pos, double dt) {

		hagame::utils::Random random;

		if (turnAroundDelay == nullptr) {

			lookingAt = random.integer(0, 2) == 1 ? Vec3::Right() : Vec3::Left();
			movementForce = lookingAt * force;

			turnAroundDelay = hagame::utils::TimedCallback<false>(random.poisson<int>(meanDelay));

			turnAroundDelay->then([this](long time) {
				turnAround();
			});
		}
	}

	void turnAround() {
		hagame::utils::Random random;
		auto delay = random.poisson<int>(meanDelay);

		turnAroundDelay = hagame::utils::TimedCallback<false>(delay);
		turnAroundDelay->then([this](long time) {
			turnAround();
		});

		auto turnAround = random.integer(0, 10);

		if (turnAround < 8) {
			lookingAt *= -1;
			movementForce = lookingAt * force;
		}
		else {
			movementForce = Vec3::Zero();
		}
		
	}

	void pursue(Vec3 pos, double dt) {
		if (target.has_value()) {
			auto delta = target.value()->getPos() - pos;
			lookingAt[0] = sign(delta[0]);
			movementForce = lookingAt * force;
		}
	}

	AIState decide(ECS* ecs, Collisions* collisions, Vec3 pos, double dt) {

		if (turnAroundDelay != nullptr) {
			turnAroundDelay->update();
		}

		float t;
		//auto hit = collisions->raycast(hagame::math::Ray(pos, lookingAt * sightDistance), ecs->entities, t, {}, { entity->uuid });

		//if (hit.has_value()) {
		//	std::cout << hit.value().entity->name << "\n";
		//}

		
		// std::cout << hagame::math::Ray(pos, lookingAt * sightDistance).toString() << "\n";
		auto hits = collisions->raycastSweep2D(
			hagame::math::Ray(pos, lookingAt * sightDistance),
			PI / 3.0f,
			5,
			ecs->entities,
			{},
			{entity->uuid}
		);

		for (auto hit : hits) {
			std::cout << hit->name << "\n";
			if (hit->hasTag("player")) {
				target = hit;
				return (hit->getPos() - pos).magnitude() > 1 ? AIState::Pursue : AIState::Attack;
			}
		}
		

		return AIState::Idle;
	}
};

#endif
