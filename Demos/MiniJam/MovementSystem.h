#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "./../../HaGame/HaGame.h"

struct PlayerMovement {
	float maxSpeed = 200.0f;
	float accel = 1000.0f;
	float revAccel = 800.0f;
	float dragAccel = 900.0f;
	Vec3 vel = Vec3::Zero();
};

class MovementSystem : public hagame::ecs::System {
public:

	String getSystemName() {
		return "MovementSystem";
	}

	void onSystemStart() {

	}

	void onSystemUpdate(double dt) {
		Vec2 mousePos = game->input.keyboardMouse.mouse.position;
		mousePos[1] = game->window->size[1] - mousePos[1];

		forEach<PlayerMovement>([this, dt, &mousePos](PlayerMovement* pm, hagame::ecs::Entity* entity) {
			Vec2 movement;
			if (game->input.gamepads.size() > 0 && game->input.gamepads[0].lAxis.magnitude() > 0) {
				movement = game->input.gamepads[0].lAxis;
			}
			else {
				movement = game->input.keyboardMouse.lAxis;
			}
			movement.normalize();

			Vec2 mouseDelta = mousePos - entity->transform->position.resize<2>();
			float angle = atan2(mouseDelta[1], mouseDelta[0]);

			entity->transform->setRotation(Quat(angle, Vec3::Face()));

			for (int i = 0; i < 2; i++) {
				if (abs(movement[i]) > 0.0) {
					bool sameDir = pm->vel[i] == 0.0 || sign(pm->vel[i]) == sign(movement[i]);
					if (sameDir) {
						if (abs(pm->vel[i]) < pm->maxSpeed) {
							pm->vel[i] += pm->accel * movement[i] * dt;
						}
					}
					else {
						if (abs(pm->vel[i]) > 0) {
							pm->vel[i] += pm->revAccel * movement[i] * dt;
						}
					}
				}
				else {
					if (abs(pm->vel[i]) * dt < EPSILON) {
						pm->vel[i] = 0.0;
					}
					else {
						pm->vel[i] -= pm->dragAccel * pm->vel.normalized()[0] * dt;
					}
				}
			}


			auto drawing = scene->getSystem<DrawingSystem>();
			auto playerCircle = Circle(Vec2({entity->transform->position[0], entity->transform->position[1]}), 12.5);

			//std::cout << playerCircle.center.toString() << std::endl;

			for (auto stroke : drawing->strokes) {
				if (stroke->boundingBox.contains((playerCircle.center).resize<2>())) {

					for (int i = 0; i < stroke->points.size(); i++) {

						for (int j = 0; j < 2; j++) {
							Vec2 velOffset;
							velOffset[j] = pm->vel[j] * dt;
							if (playerCircle.contains(stroke->points[i] - velOffset[j])) {
								pm->vel[j] = 0.0;
							}
						}

						
					}
				}
			}

			entity->transform->move(pm->vel * dt);
		});
	}
};

#endif
