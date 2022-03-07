#ifndef UI_FLEX_GRID_H
#define UI_FLEX_GRID_H

#include "./../../Utils/Aliases.h"
#include "./../../Graphics/Routines.h"
#include "./../../Core/ECS/Entity.h"
#include "./UIComponent.h"
#include "./../Common/Anchor.h"

namespace hagame {
	namespace ui {

		// FlexGrid provides a grid system where UI elements can exist in a range of the grid rather than a single cell
		class FlexGrid : public UIComponent {
		public:

			AnchorType anchor = AnchorType::Center;
			float padding = 0;
			bool display = false;


			Rect getBoundingBox() {

			}

		private:


		};
	}
}