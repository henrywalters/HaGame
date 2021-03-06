#ifndef LIGHTING_SYSTEM_H
#define LIGHTING_SYSTEM_H

#include "../../Core/Game.h"
#include "../../Core/ECS/System.h"
#include "../../Utils/Aliases.h"
#include "../Components/LightComponent.h"

namespace hagame {
	namespace graphics {
		class LightingSystem : public hagame::ecs::System {
			String getSystemName() {
				return "hagame::graphics::LightingSystem";
			}

			void onSystemBeforeUpdate(double dt) {
				forEach<LightComponent>([this](LightComponent* light, hagame::ecs::Entity* entity) {
					game->resources->getShaderProgram("texture")->use();
					game->resources->getShaderProgram("texture")->setLight("light", entity->transform->position, light->light);
				});

				forEach<DirLightComponent>([this](DirLightComponent* light, hagame::ecs::Entity* entity) {
					game->resources->getShaderProgram("texture")->use();
					game->resources->getShaderProgram("texture")->setLight("light", light->light);
				});

				forEach<PointLightComponent>([this](PointLightComponent* light, hagame::ecs::Entity* entity) {
					game->resources->getShaderProgram("texture")->use();
					game->resources->getShaderProgram("texture")->setLight("light", entity->transform->position, light->light);
				});
			}
		};
	}
}

#endif
