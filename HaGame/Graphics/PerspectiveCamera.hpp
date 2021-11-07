#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "Camera.h"
#include "Window.h"

namespace hagame {
	namespace graphics {
		class PerspectiveCamera : public Camera {
		public:

			float fov = PI / 3;
			float aspectRatio = 1.0f;
			float zNear = 0.01f;
			float zFar = 100.0f;

			Mat4 getViewMatrix(hagame::Transform *transform) {
				return Mat4::LookAt(transform->getPosition(), transform->getPosition() + transform->face(), Vec3::Top());
			}

			Mat4 getProjMatrix(hagame::Transform* transform) {
				return Mat4::Perspective(fov, aspectRatio, zNear, zFar);
			}

			void setAspectRatio(Window* window) {
				aspectRatio = (float)window->size[0] / (float)window->size[1];
			}
		};
	}
}

#endif
