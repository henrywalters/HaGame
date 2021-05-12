#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/Scene.h"
#include "../Components/MeshRenderer.h"
#include "../../Utils/Aliases.h"
#include "../../Core/ECS/Entity.h"
#include <cstddef>

namespace hagame {
	namespace graphics {
		class RenderSystem : public hagame::ecs::System {
		public:
			String getSystemName() {
				return "hagame::graphics::RenderSystem";
			}

			void onSystemUpdate(double dt) {

				auto viewMat = scene->activeCamera->getViewMatrix();

				auto projMat = scene->activeCamera->getProjMatrix();

				forEach<MeshRenderer>([this, dt, viewMat, projMat](MeshRenderer* r, hagame::ecs::Entity* entity) {
					r->shader->use();
					r->shader->setMVP(entity->transform.getModelMatrix(), viewMat, projMat);
					r->shader->setVec4("color", r->color);
					r->shader->setMaterial("material", r->material);
					
					if (r->texture) {
						r->texture->bind();
					}

					r->mesh->draw(r->shader);

					if (r->displayBounds) {
						hagame::graphics::drawCubeOutline(r->mesh->getBoundingCube(), hagame::graphics::Color::green(), r->shader);
					}
				});
			}
		};
	}
}

#endif
