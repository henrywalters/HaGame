#ifndef MOUSE_SYSTEM_HPP
#define MOUSE_SYSTEM_HPP

#include "./../../HaGame/HaGame.h"
#include "./StateSystem.hpp"
#include "./../Components/Gizmo.hpp"

class MouseSystem : public hagame::ecs::System {
public:

	Vec2 mousePos;
	Vec3 mouseWorldPos;
	hagame::math::Ray mouseWorldRay;
	std::optional<hagame::physics::Hit> rayHit;

	String getSystemName() {
		return "MouseSystem";
	}

	void onSystemUpdate(double dt) {

		auto debug = scene->getSystem<StateSystem>()->state->debug;

		if (debug) {
			mousePos = game->input.keyboardMouse.mouse.position;
		}
		else {
			mousePos = game->window->size * 0.5f;
		}
		
		mouseWorldPos = MouseToWorld(scene->viewMat, scene->projMat, mousePos, game->window->size);
		mouseWorldRay.origin = scene->activeCameraEntity->transform->getPosition();
		mouseWorldRay.direction = (mouseWorldPos - scene->activeCameraEntity->transform->getPosition()).normalized() * CAMERA_RAY_DISTANCE;
	}
};

#endif
