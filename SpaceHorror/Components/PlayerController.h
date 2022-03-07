#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

struct PlayerController {
	float movementForce = 50.0f;
	float jumpForce = 100.0f;
	Vec3 lookingAt;
};

#endif
