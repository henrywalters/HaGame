#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "./../../HaGame/HaGame.h"
#include "./../../HaGame/UI/Editable.h"

class PlayerController  {
private:

public:
	Vec3 vel;
	float walkAccel = 15.0f;
	float walkDeaccel = 60.0f;
	float runAccel = 25.0f;
	float runDeaccel = 30.0f;
	float maxWalkSpeed = 4.0f;
	float maxRunSpeed = 8.0f;
	hagame::math::Sample<float, 1000> velXData;
	hagame::math::Sample<float, 1000> velYData;

	void renderUI() {
		ImGui::DragFloat("Walk Accel", &walkAccel, 0.5, 0.0f, 100.0f);
		ImGui::DragFloat("Walk Deaccel", &walkDeaccel, 0.5, 0.01, 100.0f);
		ImGui::DragFloat("Run Accel", &runAccel, 0.5, 0.0f, 100.0f);
		ImGui::DragFloat("Run Deaccel", &runDeaccel, 0.5, 0.01, 100.0f);
		ImGui::DragFloat("Walk Speed", &maxWalkSpeed, 0.1, 0.0f, 20.0f);
		ImGui::DragFloat("Run Speed", &maxRunSpeed, 0.1, 0.01, 20.0f);

		if (vel.magnitude() > 0) {
			velXData.insert(vel[0]);
			velYData.insert(vel[1]);
		}

		float arrX[1000], arrY[1000] = {};
		int index = 0;

		for (auto vel : velXData.set) {
			arrX[index] = vel;
			index++;
		}

		index = 0;

		for (auto vel : velYData.set) {
			arrY[index] = vel;
			index++;
		}

		ImGui::PlotLines("Velocity", arrX, velXData.set.size(), 0, "", -maxRunSpeed, maxRunSpeed, ImVec2(400, 200));
		ImGui::PlotLines("Velocity", arrY, velYData.set.size(), 0, "", -maxRunSpeed, maxRunSpeed, ImVec2(400, 200));
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

			for (int i = 0; i < 2; i++) {
				if (abs(dir[i]) < EPSILON) {
					vel[i] -= vel.normalized()[i] * deaccel * dt;

					if (abs(vel[i]) < EPSILON) {
						vel[i] = 0.0f;
					}
				}
				else {
					if (dir[i] > EPSILON) {
						if (vel[i] < maxVel[i]) {
							vel[i] += dir[i] * accel * dt;
						}
						else {
							vel[i] -= deaccel * dt;
						}
					}
					else if (dir[i] < -EPSILON) {
						if (vel[i] > maxVel[i]) {
							vel[i] += dir[i] * accel * dt;
						}
						else {
							vel[i] += deaccel * dt;
						}
					}
					else {
						if (vel[i] > 0) {
							vel[i] -= deaccel * dt;
						}
						else if (vel[i] < 0) {
							vel[i] += deaccel * dt;
						}
					}
				}
			}
		}
	}
};

#endif
