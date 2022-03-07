#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "./Camera.h"

namespace hagame {
	namespace graphics {
		class OrthographicCamera : public Camera {

			float MAX_LOG_ZOOM = 10.0f;
			float ZOOM_MIDPOINT = 1.0f;
			float MIN_ZOOM = -3.0f;
			float MAX_ZOOM = 10.0f;

			float zoom = ZOOM_MIDPOINT;
			float zoomLog = 1.0f;

		public:

			Vec2 size;
			
			float zNear = 0;
			float zFar = -16777215.0;

			bool centered = true;

			OrthographicCamera() {}
			OrthographicCamera(Vec2 _size) : size(_size) {}

			Mat4 getViewMatrix(hagame::Transform* transform) {
				return Mat4::Identity();
			}

			void setZoom(float _zoom) {
				zoom = clamp(_zoom, MIN_ZOOM, MAX_ZOOM);
				zoomLog = MAX_LOG_ZOOM / (1 + pow(E, -(zoom - ZOOM_MIDPOINT)));
			}

			void zoomIn(float delta) {
				setZoom(zoom - delta);
			}

			void zoomOut(float delta) {
				setZoom(zoom + delta);
			}

			float getZoom() {
				return zoomLog;
			}

			Mat4 getProjMatrix(hagame::Transform* transform) {
				return getProjMatrix(transform->getPosition());
			}

			Mat4 getProjMatrix(Vec3 camPos) {
				if (centered) {
					return Mat4::Orthographic(
						camPos[0] - size[0] * 0.5f * zoomLog,
						camPos[0] + size[0] * 0.5f * zoomLog,
						camPos[1] - size[1] * 0.5f * zoomLog,
						camPos[1] + size[1] * 0.5f * zoomLog,
						zNear,
						zFar
					);
				}
				else {
					return Mat4::Orthographic(
						0,
						camPos[0] + size[0] * zoomLog,
						0,
						camPos[1] + size[1] * zoomLog,
						zNear,
						zFar
					);
				}
				
			}

			Vec2 getGamePos(Ptr<hagame::Transform> transform, Vec2 screenPos) {
				return (screenPos - size * 0.5f) * zoomLog + transform->getPosition().resize<2>();
			}
		};
	}
}

#endif
