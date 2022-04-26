#ifndef UI_MULTI_SELECT_H
#define UI_MULTI_SELECT_H

#include "./../../Utils/Aliases.h"
#include "./../../Graphics/Routines.h"
#include "./Grid.h"
#include "./../Common/Option.h"
#include "./../../Core/Scene.h"
#include "./../../Core/ECS/Entity.h"

namespace hagame {
	namespace ui {

		class MultiSelect : public UIComponent {
		public:

			hagame::graphics::Color color = hagame::graphics::Color::white();
			hagame::graphics::Color activeColor = hagame::graphics::Color::red();

			std::function<void(String)> onSelect = [](String value) {};
			std::function<void(String)> onHover = [](String value) {};

			MultiSelect(hagame::Scene* scene, hagame::graphics::Font* font, Vec2 size, Array<Option> options) :
				m_options(options),
				m_grid(Grid(size, 1, options.size())),
				m_font(font),
				m_scene(scene)
			{
				for (int i = 0; i < options.size(); i++) {
					auto entity = scene->addEntity();
					auto textEntity = scene->addChild(entity);
					auto btnEntity = scene->addChild(entity);

					entity->move(Vec3::Face(1.0f));

					auto text = textEntity->addComponent<hagame::graphics::TextRenderer>();
					auto messageSize = font->calculateMessageSize(options[i].label);
					// textEntity->move(messageSize * -0.5f);
					text->color = color;
					text->font = font;
					text->message = options[i].label;
					text->shader = scene->game->resources->getShaderProgram("text");
					text->fontSize = font->getFontSize();
					text->alignmentH = hagame::graphics::TextHAlignment::Center;

					m_grid.addEntity(0, i, entity, [](hagame::ecs::Entity* entity) {
						return entity->getComponentInChildren<hagame::graphics::TextRenderer>();
					});

				}
			}

			Rect getBoundingBox() {
				return m_grid.getBoundingBox();
			}

			void update(RawPtr<hagame::ecs::Entity> entity) {
				m_grid.update(entity->getPos());

				for (int i = 0; i < m_options.size(); i++) {
					auto text = m_grid.getEntity(0, i)->getComponentInChildren<hagame::graphics::TextRenderer>();
					if (m_active && m_selected == i) {
						text->color = activeColor;
					}
					else {
						text->color = color;
					}
				}
			}

			void setOptions(Array<Option> options) {
				m_options = options;
				m_grid.setRows(options.size());
			}

			void setSize(Vec2 size) {
				m_grid.setSize(size);
			}

			void activate() {
				m_active = true;
			}

			void deactivate() {
				m_active = false;
			}

			void setActive(bool active) {
				m_active = active;
			}

			void setSelection(int index) {
				if (index != m_selected || !m_active) {
					m_active = true;
					m_selected = index;
					onHover(m_options[m_selected].value);
				}
			}

			void setSelectionByMouse(Vec2 pos, bool mouseDown) {

				bool active = false;

				for (int i = 0; i < m_options.size(); i++) {
					auto bb = m_grid.getEntity(0, i)->getComponentInChildren<hagame::graphics::TextRenderer>()->getBoundingBox();
					bb.pos += m_grid.getEntity(0, i)->getPos();
					if (bb.contains(pos)) {
						setSelection(i);
						active = true;
					}
				}

				if (!active) {
					setActive(false);
				}

				if (mouseDown && m_active) {
					select();
				}
			}

			void setSelectionByDeviceState(hagame::input::DeviceState state) {
				if (m_active) {

					if (state.lAxis[1] > 0.5f || state.rAxis[1] > 0.5f) {
						if (!m_upPressed) {
							moveUp();
							m_upPressed = true;
						}
					}
					else {
						m_upPressed = false;
					}
					
					if (state.lAxis[1] < -0.5f || state.rAxis[1] < -0.5f) {
						if (!m_downPressed) {
							moveDown();
							m_downPressed = true;
						}
					}
					else {
						m_downPressed = false;
					}

					if (state.aPressed) {
						select();
					}
				}
			}

			void select() {
				onSelect(m_options[m_selected].value);
			}

			void moveUp() {
				setSelection((m_selected + 1) % m_options.size());
			}

			void moveDown() {
				setSelection(m_selected == 0 ? m_options.size() - 1: m_selected - 1);
			}

		private:
			Array<Option> m_options;
			Grid m_grid;
			bool m_upPressed = false;
			bool m_downPressed = false;
			bool m_active = false;
			int m_selected = -1;
			hagame::graphics::Font* m_font;
			hagame::Scene* m_scene;
		};
	}
}


#endif
