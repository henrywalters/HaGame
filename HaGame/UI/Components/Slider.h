#ifndef SLIDER_H
#define SLIDER_H

#include "./../../Graphics/Color.h"
#include "./../../Graphics/Components/QuadRenderer.h"
#include "./UIComponent.h"
#include "./../Common/Entity2UI.h"

using namespace hagame::graphics;
using namespace hagame::ecs;

namespace hagame {
	namespace ui {

		enum class SliderAxis {
			Horizontal,
			Vertical,
		};

		class Slider : public UIComponent {
		public:
			
			float min = 0;
			float max = 1;
			float step = 0.1;

			Ptr<Entity> slider;
			entity2ui_t getSliderUI = [](Ptr<Entity> entity) { return entity->getComponent<QuadRenderer>(); };

			Ptr<Entity> bar;
			entity2ui_t getBarUI = [](Ptr<Entity> entity) { return entity->getComponent<QuadRenderer>(); };

			SliderAxis axis = SliderAxis::Horizontal;

			Rect getBoundingBox() {
				return Rect(Vec2::Zero(), Vec2::Zero());
			}

			void update() {

			}

			void setValue(float value) {

			}

			float getValue() {

			}

		private:

			float m_value;
			Vec2 m_sliderPos;

		};
	}
}

#endif
