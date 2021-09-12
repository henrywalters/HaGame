#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "./Camera.h"

namespace hagame {
	namespace graphics {
		class OrthographicCamera : public Camera {

		public:

			Vec2 size;
			float zoom = 1.0f;
			float zFar = 10.0f;
			float zNear = -10.0f;

			OrthographicCamera() {}
			OrthographicCamera(Vec2 _size) : size(_size) {}

			Mat4 getViewMatrix(hagame::Transform* transform) {
				return Mat4::Identity();
			}

			Mat4 getProjMatrix(hagame::Transform* transform) {
				return Mat4::Orthographic(
					transform->position[0] - size[0] * 0.5f * zoom,
					transform->position[0] + size[0] * 0.5f * zoom,
					transform->position[1] - size[1] * 0.5f * zoom,
					transform->position[1] + size[1] * 0.5f * zoom,
					-1.0, 
					1.0
				);
			}

			Vec2 getGamePos(Ptr<hagame::Transform> transform, Vec2 screenPos) {
				return (screenPos - size * 0.5f) * zoom + transform->position.resize<2>();
			}
		};
	}
}

#endif
