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
#include "../Components/AnimatedSpriteRenderer.h"
#include "../Components/ParticleEmitterRenderer.h"
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

			void onSystemStart() {
				cube = std::make_shared<Mesh>(CubeMesh);
			}

			void onSystemUpdate(double dt) {

				OrthographicCamera uiCamera = OrthographicCamera(scene->game->window->size);
				Mat4 uiProjMat = uiCamera.getProjMatrix(Vec3::Zero());

				forEach<MeshRenderer>([this, dt](MeshRenderer* r, Ptr<ecs::Entity> entity) {

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
				});


				scene->ecs.entities.sortByDistance<DynamicMeshRenderer>(scene->activeCameraEntity->transform->getPosition());

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

					r->mesh->getMesh()->draw(r->displayBorder);

					if (r->displayBorder) {
						auto model = entity->transform->getTranslationMatrix() * entity->transform->getRotationMatrix() * Mat4::Scale(entity->transform->getScale() * 1.025f);
						r->borderShader->use();
						// r->borderShader->setVec3("viewPos", scene->activeCameraEntity->transform->getPosition());
						r->borderShader->setMVP(model, scene->viewMat, scene->projMat);
						r->borderShader->setVec4("color", r->borderColor);
						r->mesh->getMesh()->drawBorder(r->borderColor);
					}
				});

				forEach<RigidBodyRenderer>([this](RigidBodyRenderer* r, Ptr<ecs::Entity> entity) {
					auto rb = entity->getComponent<hagame::physics::RigidBody>();
					if (rb) {
						r->shader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);
						hagame::graphics::drawLine(hagame::math::Line(Vec3::Zero(), entity->transform->getRotation().inverse().rotatePoint(rb->vel * r->velLen)), r->velColor, r->shader);
						hagame::graphics::drawLine(hagame::math::Line(Vec3::Zero(), entity->transform->getRotation().inverse().rotatePoint(rb->accel * r->accelLen)), r->accelColor, r->shader);
					}
				});

				forEach<BoundingBoxRenderer>([this](BoundingBoxRenderer* r, Ptr<ecs::Entity> entity) {
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					Cube bb = transformBoundingBox(r->boundingBox, entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->getPosition());
					hagame::graphics::drawCubeOutline(bb, hagame::graphics::Color::green(), r->shader);
				});

				forEach<AxisRenderer>([this](AxisRenderer* r, Ptr<ecs::Entity> entity) {
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					hagame::graphics::drawLine(hagame::math::Line(entity->transform->getPosition(), entity->transform->getPosition() + (entity->transform->face() * r->axisLength)), r->zColor, r->shader);
					hagame::graphics::drawLine(hagame::math::Line(entity->transform->getPosition(), entity->transform->getPosition() + (entity->transform->right() * r->axisLength)), r->xColor, r->shader);
					hagame::graphics::drawLine(hagame::math::Line(entity->transform->getPosition(), entity->transform->getPosition() + (entity->transform->top() * r->axisLength)), r->yColor, r->shader);
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

				scene->ecs.entities.sortByDistance<SpriteRenderer>(scene->activeCameraEntity->transform->getPosition());
				scene->ecs.entities.sortByDistance<Sprite3DRenderer>(scene->activeCameraEntity->transform->getPosition());

				forEach<SpriteRenderer>([this](SpriteRenderer* r, Ptr<ecs::Entity> entity) {
					if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {

						std::cout << "Rendering sprite\n";

						r->shader->use();

						auto translation = Mat4::Translation(entity->transform->getPosition() + r->sprite->rect.pos.resize<3>());
						auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
						auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

						r->shader->setMVP(
							translation * rotation * scale,
							r->viewMat.has_value() ? r->viewMat.value() : scene->viewMat,
							r->projMat.has_value() ? r->projMat.value() : scene->projMat
						);
						r->sprite->draw(r->shader);
					}
					
				});

				forEach<Sprite3DRenderer>([this](Sprite3DRenderer* r, Ptr<ecs::Entity> entity) {
					if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {
						r->shader->use();

						auto translation = Mat4::Translation(entity->transform->getPosition() + r->sprite->pos);
			
						r->shader->setVec3("cameraUp", scene->activeCameraEntity->transform->top());
						r->shader->setVec3("cameraRight", scene->activeCameraEntity->transform->right());
						r->shader->setVec3("pos", entity->transform->getPosition());
						r->shader->setVec2("size", Vec2({ r->sprite->quad->getWidth(), r->sprite->quad->getHeight() }));
						r->shader->setMVP(translation, scene->viewMat, scene->projMat);



						r->sprite->draw();
					}
				});

				forEach<Sprite2DRenderer>([this, uiProjMat](Sprite2DRenderer* r, Ptr<ecs::Entity> entity) {
					if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {
						r->shader->use();

						auto translation = Mat4::Translation(r->sprite->pos.resize<3>(1.0f));
						//auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
						//auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

						r->shader->setMVP(
							translation,
							Mat4::Identity(),
							uiProjMat
						);
						r->sprite->draw();
					}
					});

				forEach<ParticleEmitterRenderer>([this](ParticleEmitterRenderer* r, Ptr<ecs::Entity> entity) {
					r->emitter->update(scene->game->secondsElapsed);
					r->emitter->sortByDistance(entity->transform->getPosition(), scene->activeCameraEntity->transform->getPosition());
					r->shader->use();
					r->shader->setVec3("cameraUp", scene->activeCameraEntity->transform->top());
					r->shader->setVec3("cameraRight", scene->activeCameraEntity->transform->right());
					r->shader->setVec3("pos", entity->transform->getPosition());
					r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
					r->emitter->drawUI();
					r->emitter->draw();
				});

				forEach<BoxRenderer>([this](BoxRenderer* r, Ptr<ecs::Entity> entity) {
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
