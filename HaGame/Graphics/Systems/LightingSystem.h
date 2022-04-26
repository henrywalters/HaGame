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

				Array<String> shaders = {
					"texture",
					"material"
				};

				forEach<LightComponent>([this, shaders](LightComponent* light, RawPtr<ecs::Entity> entity) {
					for (auto shader : shaders) {
						game->resources->getShaderProgram(shader)->use();
						game->resources->getShaderProgram(shader)->setLight("light", entity->transform->getPosition(), light->light);
					}
				});

				forEach<DirLightComponent>([this, shaders](DirLightComponent* light, RawPtr<ecs::Entity> entity) {
					for (auto shader : shaders) {
						game->resources->getShaderProgram(shader)->use();
						game->resources->getShaderProgram(shader)->setLight("light", light->light);
					}
				});

				forEach<PointLightComponent>([this, shaders](PointLightComponent* light, RawPtr<ecs::Entity> entity) {
					for (auto shader : shaders) {
						game->resources->getShaderProgram(shader)->use();
						game->resources->getShaderProgram(shader)->setLight("light", entity->transform->getPosition(), light->light);
					}
				});
			}
		};
	}
}

#endif
