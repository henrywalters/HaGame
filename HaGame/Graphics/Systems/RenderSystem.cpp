#include "RenderSystem.h"
#include "RenderSystem.h"

void hagame::graphics::RenderSystem::textPass(double dt)
{
	OrthographicCamera uiCamera = OrthographicCamera(scene->game->window->size);
	uiCamera.centered = false;
	Mat4 uiProjMat = uiCamera.getProjMatrix(Vec3::Zero());

	forEach<TextRenderer>([this, uiProjMat](TextRenderer* r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		r->shader->setMat4("projection", uiProjMat);
		if (r->font->getFontSize() != r->fontSize) {
			r->font->setFontSize(r->fontSize);
		}
		drawText(r->shader, r->font, r->message, r->color, entity->getPos(), r->alignmentH, r->alignmentV);
	});
}

void hagame::graphics::RenderSystem::geometryPass(double dt) {
	OrthographicCamera uiCamera = OrthographicCamera(scene->game->window->size);
	Mat4 uiProjMat = uiCamera.getProjMatrix(Vec3::Zero());

	if (scene->activeCameraEntity != nullptr) {
		scene->ecs.entities.sortByDistance<DynamicMeshRenderer>(scene->activeCameraEntity->transform->getPosition());
		scene->ecs.entities.sortByDistance<SpriteRenderer>(scene->activeCameraEntity->transform->getPosition());
		scene->ecs.entities.sortByDistance<Sprite2DRenderer>(scene->activeCameraEntity->transform->getPosition());
		scene->ecs.entities.sortByDistance<Sprite3DRenderer>(scene->activeCameraEntity->transform->getPosition());
	}

	forEach<MeshRenderer>([this, dt](MeshRenderer* r, RawPtr<ecs::Entity> entity) {

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



	forEach<DynamicMeshRenderer>([this, dt](DynamicMeshRenderer* r, RawPtr<ecs::Entity> entity) {

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

	forEach<RigidBodyRenderer>([this](RigidBodyRenderer* r, RawPtr<ecs::Entity> entity) {
		auto rb = entity->getComponent<hagame::physics::RigidBody>();
		if (rb) {
			r->shader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);
			hagame::graphics::drawLine(hagame::math::Line(Vec3::Zero(), entity->transform->getRotation().inverse().rotatePoint(rb->vel * r->velLen)), r->velColor, r->shader);
			hagame::graphics::drawLine(hagame::math::Line(Vec3::Zero(), entity->transform->getRotation().inverse().rotatePoint(rb->accel * r->accelLen)), r->accelColor, r->shader);
		}
		});

	forEach<BoundingBoxRenderer>([this](BoundingBoxRenderer* r, RawPtr<ecs::Entity> entity) {
		r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
		Cube bb = transformBoundingBox(r->boundingBox, entity->transform->getModelMatrix().resize<3, 3>(), entity->transform->getPosition());
		hagame::graphics::drawCubeOutline(bb, hagame::graphics::Color::green(), r->shader);
		});

	forEach<AxisRenderer>([this](AxisRenderer* r, RawPtr<ecs::Entity> entity) {
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

	forEach<Text3dRenderer>([this](Text3dRenderer* r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		r->shader->setMVP(entity->transform->getModelMatrix() * Mat4::Scale(Vec3(r->font->getScale())), scene->viewMat, scene->projMat);
		drawText(r->shader, r->font, r->message, r->color, Vec3::Zero());
		});




	forEach<SpriteRenderer>([this](SpriteRenderer* r, RawPtr<ecs::Entity> entity) {
		if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {

			r->shader->use();

			auto translation = Mat4::Translation(entity->transform->getPosition() + r->sprite->rect.pos);
			auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
			auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

			r->shader->setMVP(
				translation * rotation * scale,
				scene->viewMat,
				scene->projMat
			);
			r->sprite->draw(r->shader);

			glCheckError();
		}

		});

	forEach<Sprite3DRenderer>([this](Sprite3DRenderer* r, RawPtr<ecs::Entity> entity) {
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

	forEach<Sprite2DRenderer>([this, uiProjMat](Sprite2DRenderer* r, RawPtr<ecs::Entity> entity) {
		if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL && r->sprite->renderMode == RenderMode::Geometry) {
			r->shader->use();

			auto translation = Mat4::Translation(entity->getPos() + r->sprite->pos);
			//auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
			//auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

			r->shader->setVec4("color", r->sprite->color);

			r->shader->setMVP(
				translation,
				Mat4::Identity(),
				scene->projMat
			);
			r->sprite->draw();
		}
		});

	forEach<EXRRenderer>([this](RawPtr<EXRRenderer> r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		auto translation = Mat4::Translation(entity->getPos());

		r->shader->setInt("mode", 0);

		r->shader->setMVP(
			translation,
			Mat4::Identity(),
			scene->projMat
		);

		r->exr->draw();
	});

	forEach<ParticleEmitterRenderer>([this](ParticleEmitterRenderer* r, RawPtr<ecs::Entity> entity) {
		r->emitter->update(scene->game->secondsElapsed);
		r->emitter->sortByDistance(entity->transform->getPosition(), scene->activeCameraEntity->transform->getPosition());
		r->shader->use();
		r->shader->setVec3("cameraUp", scene->activeCameraEntity->transform->top());
		r->shader->setVec3("cameraRight", scene->activeCameraEntity->transform->right());
		r->shader->setVec3("pos", entity->transform->getPosition());
		r->shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
		// r->emitter->drawUI();
		r->emitter->draw();
		});

	forEach<BoxRenderer>([this](BoxRenderer* r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		r->shader->setMat4("view", scene->viewMat);
		r->shader->setMat4("projection", scene->projMat);
		//r->shader->setMVP(
		//	Mat4::Translation(entity->transform->getPosition()),
		//	scene->viewMat,
		//	scene->projMat
		//);
		// std::cout << entity->getPos() << "\n";
		r->draw(entity->transform->getPosition());
		});

	forEach<QuadRenderer>([this](QuadRenderer* r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		r->shader->setMVP(entity->transform->getModelMatrix(), scene->viewMat, scene->projMat);
		r->shader->setVec4("color", r->color);
		r->quad->getMesh()->draw();
		});

	forEach<AnimatedSpriteRenderer>([this, dt](AnimatedSpriteRenderer* r, RawPtr<ecs::Entity> entity) {
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

void hagame::graphics::RenderSystem::specularPass(double dt)
{
	forEach<EXRRenderer>([this](RawPtr<EXRRenderer> r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		auto translation = Mat4::Translation(entity->getPos());

		r->shader->setInt("mode", 1);

		r->shader->setMVP(
			translation,
			Mat4::Identity(),
			scene->projMat
		);

		r->exr->draw();
	});
}

void hagame::graphics::RenderSystem::lightPass(double dt) {

	glActiveTexture(GL_TEXTURE0);
	game->window->renderPasses.get(RenderMode::Normal)->texture->bind();

	forEach<SpriteLightRenderer>([this](SpriteLightRenderer* r, RawPtr<ecs::Entity> entity) {
		if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {
			r->shader->use();

			auto translation = Mat4::Translation(entity->getPos() + r->sprite->pos);
			//auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
			//auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());
			r->shader->setVec4("color", r->sprite->color);

			r->shader->setMVP(
				translation,
				Mat4::Identity(),
				scene->projMat
			);
			r->sprite->draw();
		}
	});

	forEach<DiscLightRenderer>([this](DiscLightRenderer* r, RawPtr<ecs::Entity> entity) {
		if (entity != NULL && r->disc != NULL) {
			r->shader->use();

			auto translation = Mat4::Translation(entity->getPos());
			//auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
			//auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

			r->shader->setVec4("color", r->color);
			r->shader->setFloat("pixelsPerMeter", game->window->pixelsPerMeter);
			r->shader->setFloat("constant", r->constant);
			r->shader->setFloat("linear", r->linear);
			r->shader->setFloat("quadratic", r->quadratic);

			// std::cout << entity->getPos() << "\n";

			r->shader->setVec4("lightPos", (((scene->projMat * entity->getPos().resize<4>(1.0f)) + Vec4(1.0f)) * 0.5));
			r->shader->setVec2("screenSize", game->window->size);

			r->shader->setMVP(
				translation,
				Mat4::Identity(),
				scene->projMat
			);

			r->disc->getMesh()->draw();
		}
	});
}

void hagame::graphics::RenderSystem::unlitPass(double dt)
{
	forEach<Sprite2DRenderer>([this](Sprite2DRenderer* r, RawPtr<ecs::Entity> entity) {
		if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL && r->sprite->renderMode == RenderMode::Unlit) {
			r->shader->use();

			auto translation = Mat4::Translation(entity->getPos() + r->sprite->pos);
			//auto rotation = Mat4::Rotation(entity->transform->getRotation() * Quat(r->sprite->rotation, Vec3::Face()));
			//auto scale = Mat4::Scale(r->sprite->rect.size.resize<3>());

			r->shader->setVec4("color", r->sprite->color);

			r->shader->setMVP(
				translation,
				Mat4::Identity(),
				scene->projMat
			);
			r->sprite->draw();
		}
	});
}

void hagame::graphics::RenderSystem::normalPass(double dt)
{
	forEach<EXRRenderer>([this](RawPtr<EXRRenderer> r, RawPtr<ecs::Entity> entity) {
		r->shader->use();
		auto translation = Mat4::Translation(entity->getPos());

		r->shader->setInt("mode", 2);

		r->shader->setMVP(
			translation,
			Mat4::Identity(),
			scene->projMat
		);

		r->exr->draw();
	});

	forEach<Sprite2DRenderer>([this](Sprite2DRenderer* r, RawPtr<ecs::Entity> entity) {
		if (entity != NULL && r->sprite->texture != NULL && r->shader != NULL) {
			
			auto shader = game->resources->getShaderProgram("sprite2d_normal");

			shader->use();

			auto translation = Mat4::Translation(entity->getPos() + r->sprite->pos);

			shader->setVec4("color", r->sprite->color);

			shader->setMVP(
				translation,
				Mat4::Identity(),
				scene->projMat
			);
			r->sprite->draw();
		}
	});

	forEach<ParticleEmitterRenderer>([this](ParticleEmitterRenderer* r, RawPtr<ecs::Entity> entity) {
		auto shader = game->resources->getShaderProgram("particle_normal");
		shader->use();
		shader->setVec3("cameraUp", scene->activeCameraEntity->transform->top());
		shader->setVec3("cameraRight", scene->activeCameraEntity->transform->right());
		shader->setVec3("pos", entity->transform->getPosition());
		shader->setMVP(Mat4::Identity(), scene->viewMat, scene->projMat);
		// r->emitter->drawUI();
		r->emitter->draw();
	});
}
