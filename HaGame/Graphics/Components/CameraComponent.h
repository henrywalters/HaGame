#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "../Camera.h"
#include "../../Core/ECS/Component.h"

namespace hagame {
	namespace graphics {
		struct CameraComponent : public hagame::ecs::Component {
			bool active = true;
			Ptr<Camera> camera;

			String getName() {
				return "Camera Component";
			}
		};
	}
}

#endif
