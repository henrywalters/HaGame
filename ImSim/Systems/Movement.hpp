#ifndef CAMERA_MOVEMENT_HPP
#define CAMERA_MOVEMENT_HPP

#include "./../../HaGame/HaGame.h"
#include "./../Components/PlayerMovement.hpp"
#include "./StateSystem.hpp"

using namespace hagame::graphics;
using namespace hagame::ecs;

struct DebugCamera {
	Vec3 pos;
	Vec2 euler = Vec2::Zero();
	float movementSpeed = 5.0f;
	float dragSpeed = 0.01f;
	float lookSpeed = 0.005f;
	float zoomSpeed = 0.05f;
};

class ImSimMovementSystem : public hagame::ecs::System {

public:

	String getSystemName() {
		return "Movement SYstem";
	}
	
	void onSystemUpdate(double dt) {

		if (scene->getSystem<StateSystem>()->state->debug) {

			forEach<DebugCamera>([this, dt](DebugCamera* dummy, Ptr<Entity> entity) {

				auto cam = entity->getComponent<CameraComponent>();

				entity->transform->move(
					entity->transform
					->right() * dummy->movementSpeed * dt * -game->input.keyboardMouse.lAxis[0]
				);

				entity->transform->move(
					entity->transform
					->face() * dummy->movementSpeed * dt * game->input.keyboardMouse.lAxis[1]
				);

				if (game->input.keyboardMouse.mouse.middle) {
					auto delta = entity->transform->top() * dummy->dragSpeed * game->input.keyboardMouse.rAxis[1] +
						entity->transform->right() * dummy->dragSpeed * -game->input.keyboardMouse.rAxis[0];

					entity->transform->move(delta);
				}

				if (game->input.keyboardMouse.mouse.right) {

					dummy->euler += game->input.keyboardMouse.rAxis * dummy->lookSpeed;
					dummy->euler[1] = clamp(dummy->euler[1], -PI / 2.0f, PI / 2.0f);
				}

				std::dynamic_pointer_cast<PerspectiveCamera>(cam->camera)->fov += game->input.keyboardMouse.mouse.wheel * dummy->zoomSpeed;

				entity->transform->setRotation(Quat(dummy->euler[0], Vec3::Top()) * Quat(dummy->euler[1], Vec3::Right()));
			});
		}
		else {
			forEach<PlayerMovement>([this, dt](PlayerMovement* pm, Ptr<Entity> entity) {
				bool running = game->input.keyboardMouse.keyboard.lShift;
				float accel = running ? pm->runAccel : pm->walkAccel;
				float speed = running ? pm->runSpeed : pm->walkSpeed;
				float strafeAccel = running ? pm->runStrafeAccel : pm->walkStrafeAccel;
				float strafeSpeed = running ? pm->runStrafeSpeed : pm->walkStrafeSpeed;

				ImGui::Begin("Player movement");

				ImGui::Text(pm->accel.toString().c_str());
				ImGui::Text(game->input.keyboardMouse.lAxis.toString().c_str());

				if (game->input.keyboardMouse.lAxis[1] != 0) {
					pm->vel += entity->transform->face() * accel * dt;
				}

				if (game->input.keyboardMouse.lAxis[0] != 0) {
					pm->vel += entity->transform->right() * strafeAccel * dt;
				}

				if (pm->vel.magnitude() > EPSILON) {
					pm->vel -= pm->vel.normalized() * pm->groundDrag * dt;
				}
				else {
					pm->vel = Vec3::Zero();
				}

				entity->transform->move(pm->vel * dt);

				ImGui::End();
			});
		}
	}
};

#endif