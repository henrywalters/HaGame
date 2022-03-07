#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "./../../Utils/Aliases.h"
#include "./../../Graphics/Routines.h"
#include "./UIComponent.h"

namespace hagame {
	namespace ui {

		class Button : public UIComponent {
		public:

			Button(Vec2 size) {
				m_rect = Rect(Vec2::Zero(), size);
			}

			Rect getBoundingBox() {
				return m_rect;
			}
			
			std::function<void(Ptr<Entity>)> onEnter = [](Ptr<Entity> entity) {};
			std::function<void(Ptr<Entity>)> onLeave = [](Ptr<Entity> entity) {};
			std::function<void(Ptr<Entity>)> onClick = [](Ptr<Entity> entity) {};

			void update(Ptr<Entity> entity, Vec2 mousePos, bool mouseDown) {

				m_rect.pos = entity->getPos();
				m_rect.pos[0] -= m_rect.size[0] * 0.5f;
				m_rect.pos[1] -= m_rect.size[1] * 0.5f;

				hagame::graphics::drawRect(m_rect, hagame::graphics::Color::green(), hagame::graphics::DEBUG_SHADER, 1);

				bool contained = m_rect.contains(mousePos);

				if (contained && !m_mouseInButton) onEnter(entity);
				if (!contained && m_mouseInButton) onLeave(entity);

				if (contained && mouseDown) {
					onClick(entity);
				}

				m_mouseInButton = contained;
			}

			bool isMouseInButton() {
				return m_mouseInButton;
			}

		private:
			Rect m_rect;
			bool m_mouseInButton = false;
		};
	}
}

#endif
