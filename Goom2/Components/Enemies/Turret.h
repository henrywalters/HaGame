#ifndef G_TURRET_H
#define G_TURRET_H

#include "./../Actor.h"

const float TURRET_SPEED = 3.5f;

const ActorDef TURRET {
	[](ActorAIContext ctx, Ptr<hagame::ecs::Entity> entity, double dt) {

		if (ctx.actor->state == "SEARCH") {
			for (auto spotted : ctx.actor->scanArea(entity, true, { "bullet", "turret" })) {
				if (spotted->hasTag("player")) {
					ctx.actor->targetId = spotted->id;
					ctx.actor->state = "AIM";
					return;
				}
			}

			entity->transform->rotate(Quat(TURRET_SPEED * dt, Vec3::Face()));
		} else if (ctx.actor->state == "AIM") {

			auto target = ctx.scene->ecs.entities.get(ctx.actor->targetId.value());

			if (ctx.actor->inSight(entity, target, true, { "bullet", "turret" })) {
				auto angle = entity->transform->top().angleBetween(target->transform->getPosition() - entity->transform->getPosition());
				if (abs(angle) > 0.01) {
					entity->transform->rotate(Quat(TURRET_SPEED * dt * sign(angle), Vec3::Face()));
				}
				else {
					ctx.actor->state = "ATTACK";
				}
			} 
			else {
				ctx.actor->state = "SEARCH";
				ctx.actor->targetId = std::nullopt;
			}
		}
		else {
			auto target = ctx.scene->ecs.entities.get(ctx.actor->targetId.value());
			if (ctx.actor->inSight(entity, target, true, { "bullet", "turret" })) {
				auto angle = entity->transform->top().angleBetween(target->transform->getPosition() - entity->transform->getPosition());
				if (abs(angle) > 0.01) {
					ctx.actor->state = "AIM";
				}
				else {
					ctx.scene->getSystem<WeaponSystem>()->fire(
						entity,
						entity->transform->getPosition(),
						target->transform->getPosition() - entity->transform->getPosition(),
						ctx.actor->weapon
					);
				}
			}
			else {
				ctx.actor->state = "SEARCH";
				ctx.actor->targetId = std::nullopt;
			}
		}
	},
	"turret",
	{ "SEARCH", "AIM", "ATTACK" },
	"SEARCH",
	120 * DEG_TO_RAD,
	2000.0f,
	100.0f,
	0.0f
};

#endif
