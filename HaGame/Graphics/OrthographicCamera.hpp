#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "./Camera.h"

namespace hagame {
	namespace graphics {
		class OrthographicCamera : public Camera {

		public:

			Vec2 _size;
			float zoom = 1.25f;

			OrthographicCamera() : _size(Vec2::Zero()) {}
			OrthographicCamera(Vec2 size) : _size(size) {}

			Mat4 getViewMatrix(hagame::Transform* transform) {
				return Mat4::Identity();
			}

			Mat4 getProjMatrix(hagame::Transform* transform) {
				return Mat4::Orthographic(
					transform->position[0] - _size[0] / (2.0 * zoom) , 
					transform->position[0] + _size[0] / (2.0 * zoom),
					transform->position[1] - _size[1] / (2.0 * zoom),
					transform->position[1] + _size[1] / (2.0 * zoom),
					-1.0, 
					1.0
				);
			}
		};
	}
}

#endif
