#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "Camera.h"

namespace hagame {
	namespace graphics {
		class PerspectiveCamera : public Camera {
		public:

			float fov = PI / 3;
			float aspectRatio = 1.0f;
			float zNear = 0.01f;
			float zFar = 1000.0f;

			Mat4 getViewMatrix() {
				return Mat4::LookAt(transform.position, transform.position + transform.face(), Vec3::Top());
			}

			Mat4 getProjMatrix() {
				return Mat4::Perspective(fov, aspectRatio, zNear, zFar);
			}
		};
	}
}

#endif
