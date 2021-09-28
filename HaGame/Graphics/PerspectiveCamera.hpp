#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "Camera.h"

namespace hagame {
	namespace graphics {
		class PerspectiveCamera : public Camera {
		public:

			float fov = PI / 3;
			float aspectRatio = 1.0f;
			float zNear = 0.1f;
			float zFar = 500.0f;

			Mat4 getViewMatrix(hagame::Transform *transform) {
				return Mat4::LookAt(transform->getPosition(), transform->getPosition() + transform->face(), Vec3::Top());
			}

			Mat4 getProjMatrix(hagame::Transform* transform) {
				return Mat4::Perspective(fov, aspectRatio, zNear, zFar);
			}
		};
	}
}

#endif
