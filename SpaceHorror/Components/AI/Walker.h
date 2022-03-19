#ifndef WALKER_H
#define WALKER_H

#include "./../../../HaGame/HaGame.h"
#include "./AI.h"

using namespace hagame::ecs;
using namespace hagame::physics;
using namespace hagame::graphics;

struct Walker : public AI {

	

	float force = 20.0f;
	float sightDistance = 5.0f;

	void idle(Vec3 pos, double dt) {

		hagame::utils::Random random;

		if (lookingAt == Vec3::Zero()) {
			lookingAt = random.integer(0, 2) == 1 ? Vec3::Right() : Vec3::Left();
		}

		auto r1 = random.normal<double>(0, 1);
		if (r1 > 2.5) {
			lookingAt[0] *= -1;
		}
		movementForce = lookingAt * force;
	}

	void pursue(Vec3 pos, double dt) {
		if (target.has_value()) {
			auto delta = target.value()->getPos() - pos;
			lookingAt[0] = sign(delta[0]);
			movementForce = lookingAt * force;
		}
	}

	AIState decide(ECS* ecs, Collisions* collisions, Vec3 pos, double dt) {
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
			// std::cout << hit->name << "\n";
			if (hit->hasTag("player")) {
				target = hit.get();
				return (hit->getPos() - pos).magnitude() > 1 ? AIState::Pursue : AIState::Attack;
			}
		}

		return AIState::Idle;
	}
};

#endif
