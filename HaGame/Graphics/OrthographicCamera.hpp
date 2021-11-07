#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "./Camera.h"

namespace hagame {
	namespace graphics {
		class OrthographicCamera : public Camera {

			float MAX_LOG_ZOOM = 10.0f;
			float ZOOM_MIDPOINT = 1.5f;
			float MIN_ZOOM = -3.0f;
			float MAX_ZOOM = 10.0f;

			float zoom = ZOOM_MIDPOINT;
			float zoomLog = 5.0f;

		public:

			Vec2 size;
			
			float zFar = 10.0f;
			float zNear = -10.0f;

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
				return Mat4::Orthographic(
					camPos[0] - size[0] * 0.5f * zoomLog,
					camPos[0] + size[0] * 0.5f * zoomLog,
					camPos[1] - size[1] * 0.5f * zoomLog,
					camPos[1] + size[1] * 0.5f * zoomLog,
					-1.0,
					1.0
				);
			}

			Vec2 getGamePos(Ptr<hagame::Transform> transform, Vec2 screenPos) {
				return (screenPos - size * 0.5f) * zoomLog+ transform->getPosition().resize<2>();
			}
		};
	}
}

#endif
