#ifndef CUSTOM_RENDER_SYSTEM_H
#define CUSTOM_RENDER_SYSTEM_H

#include "./../../HaGame/HaGame.h"
#include "./../Components/HealthBarRenderer.h"

class CustomRenderSystem : public hagame::ecs::System {
public:
	String getSystemName() {
		return "SpaceHorror::CustomRenderSystem";
	}

	void onSystemAfterUpdate(double dt) {
		forEach<HealthBarRenderer>([this](RawPtr<HealthBarRenderer> r, RawPtr<hagame::ecs::Entity> entity) {
			r->setHealth(entity->getComponent<Health>()->health);
			r->shader->use();
			r->shader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);
			r->shader->setVec4("color", r->color);
			r->getQuad()->getMesh()->draw();
		});
	}
};

#endif
