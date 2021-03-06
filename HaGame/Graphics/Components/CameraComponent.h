#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "../Camera.h"

namespace hagame {
	namespace graphics {
		struct CameraComponent {
			bool active;
			Camera* camera;
		};
	}
}

#endif
