#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include "../../HaGame/HaGame.h"

class MousePicker: public hagame::ecs::System {

	bool drawRay = true;

	void onSystemUpdate(double dt) {

		float width = game->window->getViewport().size[0];
		float height = game->window->getViewport().size[1];

		auto relPos = game->input.keyboardMouse.mouse.getRelativePosition(game->window->getViewport());

		Vec4 normMouseCoords = Vec4({
			(2.0f * relPos[0]) / width - 1.0f,
			1.0f - (2.0f * relPos[1]) / height,
			-1.0f,
			1.0f
			});

		Vec4 relOriginCoords = Vec4({ 0,0,0.1f,1.0f });
		Mat4 inverseView = scene->activeCamera->getViewMatrix().inverted();
		Mat4 inverseProj = scene->activeCamera->getProjMatrix().inverted();

		Mat4 inverseVP = inverseView * inverseProj;

		Vec4 worldCoords = inverseVP * normMouseCoords;

		if (drawRay) {

			hagame::graphics::Line line = hagame::graphics::Line(Vec3({ 0.0f, 0.0f, 0.0f }), worldCoords.resize<3>(), hagame::graphics::Color::red());
			auto shader = game->resources.getShaderProgram("color");
			shader->use();
			shader->setVec3("color", line.color.resize<3>());
			shader->setMVP(Mat4::Identity(), scene->activeCamera->getViewMatrix(), scene->activeCamera->getProjMatrix());
			line.draw(shader);
		}
		

	}
};

#endif
