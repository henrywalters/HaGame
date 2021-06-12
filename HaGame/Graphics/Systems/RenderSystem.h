#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/Scene.h"
#include "../Components/MeshRenderer.h"
#include "../../Utils/Aliases.h"
#include "../../Core/ECS/Entity.h"
#include "../Components/AxisRenderer.h"
#include "../../Physics/Components/RigidBody.h"
#include "../Components/BoundingBoxRenderer.h"
#include "../Components/SkyboxRenderer.h"
#include "../Components/TextRenderer.h"
#include "../Components/Text3dRenderer.h"
#include <cstddef>

namespace hagame {
	namespace graphics {
		class RenderSystem : public hagame::ecs::System {

			Ptr<Mesh> cube;

		public:
			String getSystemName() {
				return "hagame::graphics::RenderSystem";
			}

			void onSystemStart() {
				cube = std::make_shared<Mesh>(CubeMesh);
			}

			void onSystemUpdate(double dt) {

				forEach<MeshRenderer>([this, dt](MeshRenderer* r, hagame::ecs::Entity* entity) {
					auto model = entity->transform->getModelMatrix();
					r->shader->use();
					r->shader->setMVP(model, scene->viewMat, scene->projMat);
					r->shader->setMat4("normal", model.inverted());
					// r->shader->setVec4("color", r->color);
					// r->shader->setMaterial("material", r->material);
					
					if (r->texture) {
						glActiveTexture(GL_TEXTURE0 + 0);
						r->texture->bind();
						glActiveTexture(GL_TEXTURE0 + 1);
						r->texture->bind();
						glActiveTexture(GL_TEXTURE0 + 2);
						r->texture->bind();
					}

					if (r->specularTexture) {
						glActiveTexture(GL_TEXTURE0 + 1);
						r->specularTexture->bind();
					}

					if (r->opacityTexture) {
						glActiveTexture(GL_TEXTURE0 + 2);
						r->opacityTexture->bind();
					}

					if (r->normalMap) {
						glActiveTexture(GL_TEXTURE0 + 3);
						r->normalMap->bind();
					}

					r->mesh->draw(r->shader);
				});

				forEach<RigidBodyRenderer>([this](RigidBodyRenderer* r, hagame::ecs::Entity* entity) {
					auto rb = entity->getComponent<hagame::physics::RigidBody>();
					if (rb) {
						r->shader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);
						hagame::graphics::drawLine(hagame::graphics::Line(Vec3::Zero(), entity->transform->rotation.inverse().rotatePoint(rb->vel * r->velLen), r->velColor), r->shader);
						hagame::graphics::drawLine(hagame::graphics::Line(Vec3::Zero(), entity->transform->rotation.inverse().rotatePoint(rb->accel * r->accelLen), r->accelColor), r->shader);
					}
				});

				forEach<BoundingBoxRenderer>([this](BoundingBoxRenderer* r, hagame::ecs::Entity* entity) {
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					Cube bb = transformBoundingBox(r->boundingBox, entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->position);
					hagame::graphics::drawCubeOutline(bb, hagame::graphics::Color::green(), r->shader);
				});

				forEach<AxisRenderer>([this](AxisRenderer* r, hagame::ecs::Entity* entity) {
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					hagame::graphics::drawLine(hagame::graphics::Line(entity->transform->position, entity->transform->position + (entity->transform->face() * r->axisLength), r->zColor), r->shader);
					hagame::graphics::drawLine(hagame::graphics::Line(entity->transform->position, entity->transform->position + (entity->transform->right() * r->axisLength), r->xColor), r->shader);
					hagame::graphics::drawLine(hagame::graphics::Line(entity->transform->position, entity->transform->position + (entity->transform->top() * r->axisLength), r->yColor), r->shader);
				});

				forEach<SkyboxRenderer>([this](SkyboxRenderer* r) {
					scene->viewMat.setCol(3, Vec4::Zero());
					r->shader->use();
					r->shader->setMVP(Mat4::Scale(Vec3(20.0)), scene->viewMat, scene->projMat);
					glActiveTexture(GL_TEXTURE0 + 0);
					r->cubemap->bind();
					cube->draw(r->shader);
				});

				forEach<Text3dRenderer>([this](Text3dRenderer* r, hagame::ecs::Entity* entity) {
					r->shader->use();
					r->shader->setMVP(entity->transform->getModelMatrix() * Mat4::Scale(Vec3(r->font->getScale())), scene->viewMat, scene->projMat);
					drawText(r->shader, r->font, r->message, r->color, Vec3::Zero(), r->maxLength);
				});
			}
		};
	}
}

#endif
