#ifndef FPS_CAMERA_CONTROLLER_H
#define FPS_CAMERA_CONTROLLER_H

#include "../../Utils/Aliases.h"
#include "../Camera.h"

namespace hagame {
	namespace graphics {

		struct FPSCameraControllerParams {
			Vec2 look;
			bool zoomed;
		};

		struct FPSCameraController {
			float pitch = 0;
			float yaw = 0;
			float roll = 0;
			float minPitch = -PI / 2 + EPSILON;
			float maxPitch = PI / 2 - EPSILON;
			float ySensitivity = 1.0f;
			float xSensitivity = 1.0f;
			float zSensitivity = 1.0f;
			float tiltSensitivity = 1.0f;
			Camera* camera;
			std::function<FPSCameraControllerParams(void)> inputFn;
		};
	}
}

#endif
