#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "../../Core/ECS/System.h"
#include "../../Core/Scene.h"
#include "../Components/FPSCameraController.h"
#include "../../Utils/Aliases.h"
#include "../../Core/ECS/Entity.h"
#include "../../Physics/Components/RigidBody.h"

namespace hagame {
	namespace graphics {
		class CameraSystem : public hagame::ecs::System {
		public:
			String getSystemName() {
				return "hagame::graphics::RenderSystem";
			}

			void onSystemUpdate(double dt) {

				Ptr<ecs::Entity> camera;

				forEach<CameraComponent>([this, &camera](CameraComponent* cam, Ptr<ecs::Entity> entity) {
					if (cam->active) {
						camera = entity;
					}
				});

				forEach<FPSCameraController>([this, dt, camera](FPSCameraController* cam, Ptr<ecs::Entity> entity) {

					auto params = cam->inputFn();
					cam->yaw += params.look[0] * dt * cam->xSensitivity;
					cam->pitch += params.look[1] * dt * cam->ySensitivity;
					cam->pitch = clamp(cam->pitch, cam->minPitch, cam->maxPitch);
					
					entity->transform->setRotation(Quat(cam->yaw, Vec3::Top()));
					camera->transform->rotation = Quat(cam->yaw, Vec3::Top()) * Quat(cam->roll, Vec3::Face()) * Quat(cam->pitch, Vec3::Right());

					// ((hagame::graphics::PerspectiveCamera*) camera)->fov = params.zoomed ? PI / 5 : PI / 3;
				});
			}
		};
	}
}

#endif
