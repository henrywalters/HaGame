#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include "../../HaGame/HaGame.h"

class MousePicker: public hagame::ecs::System {

	bool drawRay = true;

	void onSystemUpdate(double dt) {
		Vec3 normMouseCoords = Vec3({
			(2.0f * game->input.keyboardMouse.mouse.position[0]) / game->window->size[0] - 1.0f,
			1.0f - (2.0f * game->input.keyboardMouse.mouse.position[1]) / game->window->size[1],
			1.0f
			});

		if (drawRay) {
			hagame::graphics::Line line = hagame::graphics::Line(Vec3::Zero(), (normMouseCoords * 1.0f), hagame::graphics::Color::red());
			std::cout << line.p1.toString() << " -> " << line.p2.toString() << std::endl;
			auto shader = game->resources.getShaderProgram("color");
			shader->use();
			shader->setVec3("color", line.color.getVec3());
			shader->setMVP(Mat4::Identity(), scene->activeCamera->getViewMatrix(), scene->activeCamera->getProjMatrix());
			line.draw(shader);
		}
		

	}
};

#endif
