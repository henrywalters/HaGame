#ifndef G_ACTOR_SYSTEM_H
#define G_ACTOR_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Components/Actor.h"

class ActorSystem : public hagame::ecs::System {
public:
	String getSystemName() {
		return "ActorSystem";
	}

	void onSystemUpdate(double dt) {
		forEach<Actor>([this, dt](Actor* actor, Ptr<hagame::ecs::Entity> entity) {
			ActorAIContext context;
			context.actor = actor;
			context.scene = scene;
			actor->weapon->update(dt);
			actor->def.ai(context, entity, dt);
		});
	}
};

#endif
