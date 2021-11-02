#ifndef CAMERA_MOVEMENT_HPP
#define CAMERA_MOVEMENT_HPP

#include "./../../HaGame/HaGame.h"

using namespace hagame::graphics;
using namespace hagame::ecs;

class CameraMovement : public hagame::ecs::System {

public:

	float movementSpeed = 5.0f;
	float dragSpeed = 0.01f;
	float lookSpeed = 0.005f;
	float zoomSpeed = 0.05f;

	Vec2 euler = Vec2::Zero();

	String getSystemName() {
		return "CameraMovement";
	}
	
	void onSystemUpdate(double dt) {
		forEach<CameraComponent>([this, dt](CameraComponent* cam, Ptr<Entity> entity) {
			entity->transform->move(
				entity->transform
					->right() * movementSpeed * dt * -game->input.keyboardMouse.lAxis[0]
			);

			entity->transform->move(
				entity->transform
					->face() * movementSpeed * dt * game->input.keyboardMouse.lAxis[1]
			);

			if (game->input.keyboardMouse.mouse.middle) {
				auto delta = entity->transform->top() * dragSpeed * game->input.keyboardMouse.rAxis[1] +
					entity->transform->right() * dragSpeed * -game->input.keyboardMouse.rAxis[0];

				entity->transform->move(delta);
			}

			if (game->input.keyboardMouse.mouse.right) {
				
				euler += game->input.keyboardMouse.rAxis * lookSpeed;
				euler[1] = clamp(euler[1], -PI / 2.0f, PI / 2.0f);
			}

			std::dynamic_pointer_cast<PerspectiveCamera>(cam->camera)->fov += game->input.keyboardMouse.mouse.wheel * zoomSpeed;

			entity->transform->setRotation(Quat(euler[0], Vec3::Top()) * Quat(euler[1], Vec3::Right()));
		});
	}
};

#endif