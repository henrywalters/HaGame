#ifndef PLAYER_MOVEMENT_HPP
#define PLAYER_MOVEMENT_HPP

#include "./../../HaGame/HaGame.h"

struct PlayerMovement {

	// Dependencies
	hagame::physics::Collider* groundCollider;

	// Configurables 
	float walkAccel = 30.0f;
	float runAccel = 80.0f;
	float walkStrafeAccel = 35.0f;
	float runStrafeAccel = 100.0f;
	float minSpeed = 0.1f;
	float jumpHeight = 1.0f;
	float groundDrag = 8.0f;
	float airDrag = 10.0f;
	float stepHeight = 0.5f;
	float groundCheckHeight = 0.2f;
	Vec3 gravityAccel = Vec3({ 0.0f, -1.0f, 0.0f });

	// State
	Vec3 vel;
	bool isGrounded;

	void drawUI() {
		ImGui::Text(("Grounded: " + std::to_string(isGrounded)).c_str());

		ImGui::SliderFloat("Walk Accel", &walkAccel, 0.0f, 1000.0f);
		ImGui::SliderFloat("Run Accel", &runAccel, 0.0f, 1000.0f);
		ImGui::SliderFloat("Walk Strafe Accel", &walkStrafeAccel, 0.0f, 1000.0f);
		ImGui::SliderFloat("Run Strafe Accel", &runStrafeAccel, 0.0f, 1000.0f);
		ImGui::SliderFloat("Min Speed", &minSpeed, 0.0f, 1.0f);
		ImGui::SliderFloat("Ground Drag", &groundDrag, 0.0f, 50.0f);
		ImGui::SliderFloat("Air Drag", &airDrag, 0.0f, 50.0f);
		ImGui::SliderFloat("Jump Height", &jumpHeight, 0.0f, 10.0f);
		ImGui::SliderFloat3("Gravity Accel", gravityAccel.vector, -10.0f, 10.0f);
	}
};

#endif
