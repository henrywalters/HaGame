#ifndef HS_STAR_SYSTEM_H
#define HS_STAR_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Components/Star.h"

class StarSystem : public hagame::ecs::System {
public:

	String getSystemName() {
		return "StarSystem";
	}

	void onSystemUpdate(double dt) {
		forEach<Star>([this, dt](Star* star, Ptr<hagame::ecs::Entity> entity) {
			star->update(dt);
			entity->getComponent<hagame::graphics::QuadRenderer>()->color = star->color;
		});
	}
};

#endif
