#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/Scene.h"
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
#include "../Components/AnimatedSpriteRenderer.h"
#include "../../Utils/Timer.h"
#include "../../Math/Sample.h"
#include <cstddef>

namespace hagame {
	namespace graphics {
		class RenderSystem : public hagame::ecs::System {

			Ptr<Mesh> cube;
			math::Sample<float, 10000> meshRenders;
			utils::Timer timer;

		public:
			String getSystemName() {
				return "hagame::graphics::RenderSystem";
			}

			void onSystemStart() {
				cube = std::make_shared<Mesh>(CubeMesh);
				meshRenders.onFull = [this]() {
					// std::cout << meshRenders.average() << "\n";
					meshRenders.clear();
				};
			}

			void onSystemUpdate(double dt) {

				forEach<MeshRenderer>([this, dt](MeshRenderer* r, Ptr<ecs::Entity> entity) {

					timer.reset();

					r->shader->use();

					auto model = entity->transform->getModelMatrix();
					auto modelInverse = model.inverted();
					
					r->shader->setMat4("model", entity->transform->model);
					r->shader->setVec3("viewPos", scene->activeCameraEntity->transform->getPosition());
					r->shader->setMVP(model, scene->viewMat, scene->projMat);
					r->shader->setMat4("normal", modelInverse);
					r->shader->setVec4("color", r->color);
					r->shader->setMaterial("material", r->material);
					
					
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

					meshRenders.insert(timer.elapsed());
				});

				forEach<DynamicMeshRenderer>([this, dt](DynamicMeshRenderer* r, Ptr<ecs::Entity> entity) {

					r->shader->use();

					auto model = entity->transform->getModelMatrix();
					auto modelInverse = model.inverted();

					r->shader->setVec3("viewPos", scene->activeCameraEntity->transform->getPosition());
					r->shader->setMVP(model, scene->viewMat, scene->projMat);
					r->shader->setMat4("normal", modelInverse);
					r->shader->setVec4("color", r->color);
					r->shader->setMaterial("material", r->material);


					if (r->texture) {
						glActiveTexture(GL_TEXTURE0 + 0);
						r->texture->bind();
						glActiveTexture(GL_TEXTURE0 + 1);
						r->texture->bind();
						glActiveTexture(GL_TEXTURE0 + 2);
						r->texture->bind();
					}

					r->mesh->getMesh()->draw(r->shader, r->displayBorder);

					if (r->displayBorder) {
						auto model = entity->transform->getTranslationMatrix() * entity->transform->getRotationMatrix() * Mat4::Scale(entity->transform->getScale() * 1.1f);
						r->borderShader->use();
						// r->borderShader->setVec3("viewPos", scene->activeCameraEntity->transform->getPosition());
						r->borderShader->setMVP(model, scene->viewMat, scene->projMat);
						r->borderShader->setVec4("color", r->borderColor);
						r->mesh->getMesh()->drawBorder(r->borderShader, r->borderColor);
					}
				});

				forEach<RigidBodyRenderer>([this](RigidBodyRenderer* r, Ptr<ecs::Entity> entity) {
					auto rb = entity->getComponent<hagame::physics::RigidBody>();
					if (rb) {
						r->shader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);
						hagame::graphics::drawLine(hagame::graphics::Line(Vec3::Zero(), entity->transform->getRotation().inverse().rotatePoint(rb->vel * r->velLen), r->velColor), r->shader);
						hagame::graphics::drawLine(hagame::graphics::Line(Vec3::Zero(), entity->transform->getRotation().inverse().rotatePoint(rb->accel * r->accelLen), r->accelColor), r->shader);
					}
				});

				forEach<BoundingBoxRenderer>([this](BoundingBoxRenderer* r, Ptr<ecs::Entity> entity) {
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					Cube bb = transformBoundingBox(r->boundingBox, entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->getPosition());
					hagame::graphics::drawCubeOutline(bb, hagame::graphics::Color::green(), r->shader);
				});

				forEach<AxisRenderer>([this](AxisRenderer* r, Ptr<ecs::Entity> entity) {
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					hagame::graphics::drawLine(hagame::graphics::Line(entity->transform->getPosition(), entity->transform->getPosition() + (entity->transform->face() * r->axisLength), r->zColor), r->shader);
					hagame::graphics::drawLine(hagame::graphics::Line(entity->transform->getPosition(), entity->transform->getPosition() + (entity->transform->right() * r->axisLength), r->xColor), r->shader);
					hagame::graphics::drawLine(hagame::graphics::Line(entity->transform->getPosition(), entity->transform->getPosition() + (entity->transform->top() * r->axisLength), r->yColor), r->shader);
				});

				forEach<SkyboxRenderer>([this](SkyboxRenderer* r) {
					scene->viewMat.setCol(3, Vec4::Zero());
					r->shader->use();
					r->shader->setMVP(Mat4::Scale(Vec3(20.0)), scene->viewMat, scene->projMat);
					glActiveTexture(GL_TEXTURE0 + 0);
					r->cubemap->bind();
					cube->draw(r->shader);
				});

				forEach<Text3dRenderer>([this](Text3dRenderer* r, Ptr<ecs::Entity> entity) {
					r->shader->use();
					r->shader->setMVP(entity->transform->getModelMatrix() * Mat4::Scale(Vec3(r->font->getScale())), scene->viewMat, scene->projMat);
					drawText(r->shader, r->font, r->message, r->color, Vec3::Zero(), r->maxLength);
				});

				forEach<SpriteRenderer>([this](SpriteRenderer* r, Ptr<ecs::Entity> entity) {
					if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {
						r->shader->use();

						auto translation = Mat4::Translation(entity->transform->getPosition() + r->sprite->rect.pos.resize<3>());
						auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
						auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

						r->shader->setMVP(
							translation * rotation * scale,
							scene->viewMat,
							scene->projMat
						);
						r->sprite->draw(r->shader);
					}
					
				});

				forEach<BoxRenderer>([this](BoxRenderer* r, Ptr<ecs::Entity> entity) {
					std::cout << entity->id << "\n";
					r->shader->use();
					r->shader->setMat4("view", scene->viewMat);
					r->shader->setMat4("projection", scene->projMat);
					//r->shader->setMVP(
					//	Mat4::Translation(entity->transform->getPosition() + r->box.pos.resize<3>())  * Mat4::Scale(r->box.size.resize<3>()),
					//	scene->viewMat,
					//	scene->projMat
					//);
					r->draw(entity->transform->getPosition());
				});

				forEach<AnimatedSpriteRenderer>([this, dt](AnimatedSpriteRenderer* r, Ptr<ecs::Entity> entity) {
					if (r->sprites->hasActive()) {
						std::cout << r->sprites->active()->rect.size.toString() << "\n";
						r->sprites->active()->update(dt);
						r->shader->use();
						r->shader->setMVP(
							Mat4::Translation(
								entity->transform->getPosition() + r->sprites->active()->rect.pos.resize<3>()) * Mat4::Rotation(entity->transform->getRotation()) * Mat4::Scale(r->sprites->active()->rect.size.resize<3>()),
							scene->viewMat,
							scene->projMat
						);
						r->sprites->active()->draw(r->shader);
						glCheckError();
					}

				});
			}
		};
	}
}

#endif
