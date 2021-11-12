#ifndef PLAYER_MOVEMENT_HPP
#define PLAYER_MOVEMENT_HPP

#include "./../../HaGame/HaGame.h"

struct PlayerMovement {

	float walkAccel = 5.0f;
	float runAccel = 10.0f;
	float walkStrafeAccel = 10.0f;
	float runStrafeAccel = 20.0f;
	float walkSpeed = 1.0f;
	float runSpeed = 2.0f;
	float walkStrafeSpeed = 1.0f;
	float runStrafeSpeed = 2.0f;
	float jumpHeight = 1.0f;
	float groundDrag = 0.5f;
	float airDrag = 0.1f;

	Vec3 accel;
	Vec3 vel;
};

#endif
