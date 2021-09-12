#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "./../../HaGame/HaGame.h"
#include "./../../HaGame/UI/Editable.h"

class PlayerController  {

public:
	Vec3 vel;
	float walkAccel = 800.0f;
	float walkDeaccel = 700.0f;
	float runAccel = 1400.0f;
	float runDeaccel = 1200.0f;
	float maxWalkSpeed = 100.0f;
	float maxRunSpeed = 400.0f;

	void renderUI() {

	}

	void update(Vec2 dir, bool running, double dt) {
		auto velMag = vel.magnitude();
		auto dirMag = dir.magnitude();

		auto accel = running ? runAccel : walkAccel;
		auto deaccel = running ? runDeaccel : walkDeaccel;
		auto maxSpeed = running ? maxRunSpeed : maxWalkSpeed;

		if (velMag < maxWalkSpeed / 10.0) {
			if (dirMag < EPSILON) {
				vel = Vec3::Zero();
			}
			else {
				vel += dir.resize<3>() * accel * dt;
			}
		}
		else {

			Vec2 maxVel = dir.normalized() * maxSpeed;

			if (dirMag < EPSILON ) {
				vel -= vel.normalized() * deaccel * dt;
			}
			else {
				if (dir[0] > 0) {
					if (vel[0] <= maxVel[0]) {
						vel[0] += dir[0] * accel * dt;
					}
					else {
						vel[0] -= deaccel * dt;
					}
				}
				else if (dir[0] == 0) {
					if (vel[0] > 0) {
						vel[0] -= deaccel * dt;
					}
					else if (vel[0] < 0) {
						vel[0] += deaccel * dt;
					}
				}
				else {
					
					if (vel[0] > maxVel[0]) {
						vel[0] += dir[0] * accel  * dt;
					}
					else {
						vel[0] += deaccel * dt;
					}
				}

				if (dir[1] > 0) {
					if (vel[1] <= maxVel[1]) {
						vel[1] += dir[1] * accel * dt;
					}
					else {
						vel[1] -= deaccel * dt;
					}
				}
				else if (dir[1] == 0) {
					if (vel[1] > 0) {
						vel[1] -= deaccel * dt;
					}
					else if (vel[1] < 0) {
						vel[1] += deaccel * dt;
					}
				}
				else {
					if (vel[1] > maxVel[1]) {
						vel[1] += accel * dir[1] * dt;
					}
					else {
						vel[1] += deaccel * dt;
					}
				}
			}
		}
	}
};

#endif
