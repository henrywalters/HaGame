#ifndef SH_AI_H
#define SH_AI_H

#include "./../../../HaGame/HaGame.h"

enum class AIState {
	Idle,
	Pursue,
	Attack,
	Custom
};

class AI;

typedef std::function<void(AI*, Vec3, double)> ai_custom_action_t;
typedef std::function<void(Vec3, double)> ai_action_t;

class AI {
public:

	void update(RawPtr<hagame::ecs::Entity> _entity, hagame::ecs::ECS* ecs, hagame::physics::Collisions* collisions, Vec3 pos, double dt) {
		entity = _entity;
		auto state = decide(ecs, collisions, pos, dt);
		// std::cout << (int)state << "\n";
		switch (state) {
		case AIState::Idle:
			idle(pos, dt);
			break;
		case AIState::Pursue:
			pursue(pos, dt);
			break;
		case AIState::Attack:
			attack(pos, dt);
			break;
		case AIState::Custom:
			custom(decideCustom(ecs, collisions, pos, dt), pos, dt);
			break;
		}
	}

	Vec3 getMovementForce() { return movementForce; }

	void renderLineOfSight(Vec3 pos) {
		auto fovDegrees = (int)(fov * RAD_TO_DEG) * 2;
		auto step = fov / (float)fovDegrees;
		auto startAngle = atan2f(lookingAt[1], lookingAt[0]) - fov * 0.5f;

		for (int i = 0; i < fovDegrees; i++) {
			auto angle = startAngle + i * step;
			auto dir = Vec3(cos(angle) * sightDistance, sin(angle) * sightDistance);
			hagame::graphics::drawLine(hagame::math::Line(pos, pos + dir), lineOfSightColor);
		}
	}

protected:

	Map<String, ai_custom_action_t*> customStates;
	Optional<hagame::ecs::Entity*> target;
	Vec3 lookingAt;
	Vec3 movementForce;
	float fov = PI / 3.0f;
	float sightDistance = 5.0f;
	Color lineOfSightColor = Color(0.0f, 1.0f, 0.0f, 0.5f);
	hagame::ecs::Entity* entity;

	// All AI components must override the function to determine its current state
	virtual AIState decide(hagame::ecs::ECS* ecs, hagame::physics::Collisions* collisions, Vec3 pos, double dt) = 0;

	// If the custom state is returned by decide, this function will be called to determine the custom state
	virtual String decideCustom(hagame::ecs::ECS* ecs, hagame::physics::Collisions* collisions, Vec3 pos, double dt) {
		return "";
	}

	virtual void idle(Vec3 pos, double dt) {}
	virtual void pursue(Vec3 pos, double dt) {}
	virtual void attack(Vec3 pos, double dt) {}

	void custom(String state, Vec3 pos, double dt) {
		if (customStates.find(state) != customStates.end()) {
			// customStates[state](this, pos, dt);
		}
		else {
			std::cout << "AI WARNING: IGNORING CUSTOM STATE: " << state << "\n";
		}
	}


};

#endif
