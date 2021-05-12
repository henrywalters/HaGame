#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "../../HaGame/HaGame.h"

class CameraController : public hagame::ecs::System {
public:

	String getSystemName() {
		return "Camera Controller System";
	}

	void onSystemBeforeUpdate() {
		auto shader = game->resources->getShaderProgram("material");
		shader->use();
		shader->setVec3("viewPos", scene->activeCamera->transform.position);
	}

	void onSystemUpdate(double dt) {

		if (game->input.keyboardMouse.mouse.left && game->window->inViewport(game->input.keyboardMouse.mouse.position)) {
			scene->activeCamera->transform.move(Vec3::Top() * game->input.keyboardMouse.rAxis[1] * dt * 10.0f);
			scene->activeCamera->transform.move(scene->activeCamera->transform.right() * game->input.keyboardMouse.rAxis[0] * dt * 10.0f);
		}

		if (game->input.keyboardMouse.mouse.right && game->window->inViewport(game->input.keyboardMouse.mouse.position)) {
			scene->activeCamera->transform.rotate(Quat(game->input.keyboardMouse.rAxis[1] * -dt, Vec3::Right()));
			scene->activeCamera->transform.rotate(Quat(game->input.keyboardMouse.rAxis[0] * dt, Vec3::Top()));
		}

		scene->activeCamera->transform.move(scene->activeCamera->transform.face() * game->input.keyboardMouse.lAxis[1] * dt * -100);
		scene->activeCamera->transform.move(scene->activeCamera->transform.right() * game->input.keyboardMouse.lAxis[0] * dt * -50);
	}
};

#endif
