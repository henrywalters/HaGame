#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "./Camera.h"

namespace hagame {
	namespace graphics {
		class OrthographicCamera : public Camera {

		public:

			Vec2 _size;

			OrthographicCamera() : _size(Vec2::Zero()) {}
			OrthographicCamera(Vec2 size) : _size(size) {}

			Mat4 getViewMatrix(hagame::Transform* transform) {
				return Mat4::Identity();
			}

			Mat4 getProjMatrix(hagame::Transform* transform) {
				return Mat4::Orthographic(
					transform->position[0] - _size[0] / 2.0, 
					transform->position[0] + _size[0] / 2.0,		
					transform->position[1] - _size[1] / 2.0,
					transform->position[1] + _size[1] / 2.0,
					-1.0, 
					1.0
				);
			}
		};
	}
}

#endif
