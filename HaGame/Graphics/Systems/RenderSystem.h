#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/Scene.h"
#include "../../Core/Game.h"
#include "../Components/MeshRenderer.h"
#include "../Components/DynamicMeshRenderer.h"
#include "../../Utils/Aliases.h"
#include "../../Core/ECS/Entity.h"
#include "../Components/AxisRenderer.h"
#include "../../Physics/Components/RigidBody.h"
#include "../Components/BoundingBoxRenderer.h"
#include "../Components/SkyboxRenderer.h"
#include "../Components/TextRenderer.h"
#include "../Components/Text3dRenderer.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Sprite2DRenderer.h"
#include "../Components/Sprite3DRenderer.h"
#include "../Components/SpriteLightRenderer.h"
#include "../Components/DiscLightRenderer.h"
#include "../Components/EXRRenderer.h"
#include "../Components/AnimatedSpriteRenderer.h"
#include "../Components/ParticleEmitterRenderer.h"
#include "../Components/QuadRenderer.h"
#include "../../Utils/Timer.h"
#include "../../Math/Sample.h"
#include <cstddef>

namespace hagame {
	namespace graphics {
		class RenderSystem : public hagame::ecs::System {

			Ptr<Mesh> cube;

		public:
			String getSystemName() {
				return "hagame::graphics::RenderSystem";
			}

			void textPass(double dt);
			void geometryPass(double dt);
			void specularPass(double dt);
			void lightPass(double dt);
			void unlitPass(double dt);
			void normalPass(double dt);

			void onSystemStart() {
				cube = std::make_shared<Mesh>(CubeMesh);
			}

			void onSystemAfterUpdate(double dt) {

				game->window->renderPasses.bind(RenderMode::Light);
				game->window->resetGl();
				
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glDepthFunc(GL_LEQUAL);
				//game->window->activateLightBuffer();
				lightPass(dt);

				//game->window->activateGeometryBuffer();
				game->window->renderPasses.bind(RenderMode::Unlit);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthFunc(GL_LESS);
				textPass(dt);
			}

			void onSystemBeforeUpdate(double dt) {
				//game->window->clearGeometry();
				//game->window->clearLight();
				//game->window->activateGeometryBuffer();
				game->window->renderPasses.clear(RenderMode::Geometry, Color::black());
				game->window->resetGl();
				game->window->renderPasses.clear(RenderMode::Specular, game->window->clearColor);
				game->window->resetGl();
				game->window->renderPasses.clear(RenderMode::Unlit, game->window->clearColor);
				game->window->resetGl();
				game->window->renderPasses.clear(RenderMode::Light, game->window->ambientColor);
				game->window->resetGl();
				game->window->renderPasses.clear(RenderMode::Normal, Color::black());
				game->window->resetGl();

				game->window->renderPasses.bind(RenderMode::Geometry);
				game->window->renderPasses.clear(RenderMode::Geometry, Color::black());
				geometryPass(dt);

				game->window->renderPasses.bind(RenderMode::Specular);
				specularPass(dt);

				game->window->renderPasses.bind(RenderMode::Unlit);
				unlitPass(dt);

				game->window->renderPasses.bind(RenderMode::Normal);
				game->window->renderPasses.clear(RenderMode::Normal, Color::black());
				game->window->resetGl();
				normalPass(dt);

				// Stuff drawn outside of the render system assumed to be debug info (imgui, etc) so no need to apply lighting.
				game->window->renderPasses.bind(RenderMode::Unlit);
			}

			void onSystemPostTick() {

				game->window->renderPasses.render(RenderMode::Geometry);
				game->window->renderPasses.render(RenderMode::Specular);
				game->window->renderPasses.render(RenderMode::Normal);
				game->window->renderPasses.bind(RenderMode::Light);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glDepthFunc(GL_LEQUAL);
				game->window->renderPasses.render(RenderMode::Light);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthFunc(GL_LESS);
				game->window->renderPasses.render(RenderMode::Unlit);
				//game->window->renderGeometry();
				//game->window->renderLight();

				auto shader = game->resources->getShaderProgram("screen");
				shader->use();
				shader->setInt("mode", (int) game->window->renderPassMode);

				game->window->render();
			}
		};
	}
}

#endif
