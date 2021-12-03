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
	float dragSpeed = 5.0f;
	float lookSpeed = 2.0f;
	float zoomSpeed = 0.5f;
};

class ImSimMovementSystem : public hagame::ecs::System {

	double physicsDt;

public:

	String getSystemName() {
		return "Movement SYstem";
	}

	void onSystemBeforeUpdate() {
		ImGui::Begin("Player movement");
	}

	void onSystemAfterUpdate() {
		ImGui::End();
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

					entity->transform->move(delta * dt);
				}

				if (game->input.keyboardMouse.mouse.right) {

					dummy->euler += game->input.keyboardMouse.rAxis * dummy->lookSpeed * dt;
					dummy->euler[1] = clamp(dummy->euler[1], -PI / 2.0f, PI / 2.0f);
				}

				std::dynamic_pointer_cast<PerspectiveCamera>(cam->camera)->fov += game->input.keyboardMouse.mouse.wheel * dummy->zoomSpeed;

				entity->transform->setRotation(Quat(dummy->euler[0], Vec3::Top()) * Quat(dummy->euler[1], Vec3::Right()));
				});
		} else {
			forEach<PlayerMovement>([this, dt](PlayerMovement* pm, Ptr<Entity> entity) {

				// checkIfGrounded(entity, pm, dt);

				physicsDt = dt;

				bool running = !game->input.keyboardMouse.keyboard.lShift;
				float accel = running ? pm->runAccel : pm->walkAccel;
				float strafeAccel = running ? pm->runStrafeAccel : pm->walkStrafeAccel;
				float dragCoef = pm->groundDrag;

				Vec3 movementDir = Vec3({ -game->input.player(0).lAxis[0], 0.0f, game->input.player(0).lAxis[1] });

				if (movementDir.magnitude() > 1.0f)
					movementDir.normalize();

				entity->transform->move(pm->vel * dt);

				Vec3 acceleration;

				if (movementDir.z() != 0) {
					acceleration += entity->transform->face() * movementDir.z() * accel;
				}

				if (movementDir.x() != 0) {
					acceleration += entity->transform->right() * movementDir.x() * strafeAccel;
				}

				pm->vel += acceleration * dt;

				auto velMag = pm->vel.magnitude();

				if (velMag > 0) {
					for (int i = 0; i < 3; i++) {
						float velSq = pm->vel[i] * pm->vel[i];
						float drag = pm->vel[i] * dragCoef * -1;
						pm->vel[i] += drag * dt;

						if (abs(pm->vel[i]) < pm->minSpeed && movementDir[i] == 0.0f) {
							pm->vel[i] = 0.0f;
						}
					}
				}

			});
		}

		forEach<PlayerMovement>([this, dt](PlayerMovement* pm, Ptr<Entity> entity) {

			pm->drawUI();

			ImGui::Text(("DT = " + std::to_string(physicsDt)).c_str());
			ImGui::Text(("Velocity: " + pm->vel.toString()).c_str());
			ImGui::Text(("Position: " + entity->transform->getPosition().toString()).c_str());
			ImGui::Text(("Face: " + entity->transform->face().toString()).c_str());
			ImGui::Text(("RAxis: " + game->input.keyboardMouse.rAxis.toString()).c_str());
		});

	}

	void checkIfGrounded(Ptr<Entity> entity, PlayerMovement* pm, double dt) {
		float t;
		auto hit = game->collisions.checkCollisions(entity, pm->groundCollider, pm->vel, dt, t);
		pm->isGrounded = hit.has_value();
	}

};

#endif