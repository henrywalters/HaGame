#ifndef G_ACTOR_H
#define G_ACTOR_H

#include "./../../HaGame/HaGame.h"
#include "./Weapon.h"
#include "./../Systems/WeaponSystem.h"

struct Actor;

struct ActorAIContext {
	Actor* actor;
	hagame::Scene* scene;
};

using ActorAIFunc = std::function<void(ActorAIContext, Ptr<hagame::ecs::Entity>, double)>;

struct ActorDef {
	// Define this function with how the actor should think
	ActorAIFunc ai = [this](ActorAIContext ctx, Ptr<hagame::ecs::Entity> entity, double dt) {};

	// The tag name assigned to the entity
	String tagName;

	// Define the possible set of states an actor may be in
	Set<String> states = {};

	String defaultState;

	// Field of view for the actor. Used to determine what an actor can see in the world. For reference, humans have ~120 degree fov.
	float fov = 120 * DEG_TO_RAD;

	// How far can the actor see ahead of them?
	float sightDistance;

	// Max health of the actor
	float maxHealth = 100;

	// Max shield of the actor
	float maxShield = 0;
};

struct Actor {

	ActorDef def;

	hagame::Game* game;

	// The actual entity this actor is controlling
	// Ptr<hagame::ecs::Entity> entity = NULL;

	// Is the actor currently focused on another entity?
	Optional<uint32_t> targetId = std::nullopt;

	// Does the actor have a weapon
	// TODO: Implement a general inventory system for this
	Ptr<Weapon> weapon = NULL;

	// What state is the actor in. E.g. Searching, attacking, idle, etc
	String state;

	Actor() {
		state = def.defaultState;
	}

	~Actor() {
		// entity.reset();
	}

	// Checks to see if the actor entity can see the target
	bool inSight(Ptr<hagame::ecs::Entity> entity, Ptr<hagame::ecs::Entity> target, bool draw = false, Array<String> ignoreTags = {}) {
		Vec3 delta = target->transform->getPosition() - entity->transform->getPosition();
		float angle = entity->transform->top().angleBetween(delta);
		float t;

		hagame::math::Ray ray(entity->transform->getPosition(), delta.normalized() * def.sightDistance);
		Optional<Ptr<hagame::ecs::Entity>> inSight;
		inSight = game->collisions.raycast(entity, ray, t, ignoreTags);
		if (!inSight.has_value())
			t = 1.0f;

		if (draw) {
			hagame::graphics::drawLine(hagame::graphics::Line(
				entity->transform->getPosition(),
				ray.getPointOnLine(t),
				hagame::graphics::Color(1.0f, 0.0f, 0.0f, 0.2f)
			), game->resources->getShaderProgram("color"));
		}

		return inSight.has_value() && inSight.value()->id == target->id;
	}

	// Scan the area for entities the actor can see. The number of raycasts is calculated by ceil(RAD_TO_DEG * fov * precision). Pass in a shader to visualize the scan
	Array<Ptr<hagame::ecs::Entity>> scanArea(Ptr<hagame::ecs::Entity> entity, bool draw = false, Array<String> ignoreTags = {}, float precision = 30.0f) {
		Array<Ptr<hagame::ecs::Entity>> output;
		Vec3 targetPoint = entity->transform->top() * def.sightDistance;
		int raycasts = ceil(RAD_TO_DEG * def.fov * (1 / precision));

		// Ensure an odd amount of raycasts
		if (raycasts % 2 == 0)
			raycasts++;

		float gapRadius = def.fov / (raycasts - 1);
		float angle, t;

		for (int i = 0; i < raycasts; i++) {
			angle = (-def.fov / 2.0f) + i * gapRadius;

			hagame::math::Ray ray(entity->transform->getPosition(), Quat(angle, Vec3::Face()).rotatePoint(targetPoint));

			Optional<Ptr<hagame::ecs::Entity>> inSight;
			inSight = game->collisions.raycast(entity, ray, t, ignoreTags);

			if (inSight.has_value()) {
				output.push_back(inSight.value());
			}
			else {
				t = 1.0f;
			}

			if (draw) {
				hagame::graphics::drawLine(hagame::graphics::Line(
					entity->transform->getPosition(),
					ray.getPointOnLine(t),
					hagame::graphics::Color(0.0f, 1.0f, 1.0f, 0.2f)
				), game->resources->getShaderProgram("color"));
			}
		}

		return output;
	}
};


#endif
