#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "./../../HaGame/HaGame.h"

struct PlayerController {
	float movementForce = 25.0f;
	float jumpForce = 200.0f;
	Vec3 lookingAt;

	// Add a bit of a buffer where an input to jump is still valid if entered briefly before collision
	float jumpBufferSeconds = 0.5f;

	double timeOfLastJump = -1;

	void updateImGui() {
		ImGui::DragFloat("Movement Force", &movementForce, 1.0f, 0.0f, 1000.0f);
		ImGui::DragFloat("Jump Force", &jumpForce, 1.0f, 0.0f, 1000.0f);
		ImGui::DragFloat("Jump Buffer", &jumpBufferSeconds, 0.05f, 0.0f, 0.5f);
		ImGui::InputFloat3("Looking at", lookingAt.vector);
	}
};

#endif
