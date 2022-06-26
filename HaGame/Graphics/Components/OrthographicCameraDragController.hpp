#ifndef ORTHOGRAPHIC_CAMERA_DRAG_CONTROLLER
#define ORTHOGRAPHIC_CAMERA_DRAG_CONTROLLER

#include "./../OrthographicCamera.hpp"

namespace hagame {
	namespace graphics {
		class OrthographicCameraDragController {
		public:
			void update(RawPtr<hagame::ecs::Entity> entity, Vec2 inputPos, bool dragging) {

				if (dragging && !m_dragging) {
					m_dragging = true;
					m_dragStart = inputPos;
					m_dragOffset = entity->getPos();
				}

				if (!dragging) {
					m_dragging = false;
				}

				if (m_dragging) {
					auto delta = inputPos - m_dragStart;
					entity->setPos(m_dragOffset - delta);
				}
			}

			bool isDragging() const {
				return m_dragging;
			}

		private:
			bool m_dragging = false;
			Vec2 m_dragStart;
			Vec2 m_dragOffset;
		};
	}
}

#endif
