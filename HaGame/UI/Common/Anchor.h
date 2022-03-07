#ifndef UI_ANCHOR_H
#define UI_ANCHOR_H

#include "./../../Utils/Aliases.h"

namespace hagame {
	namespace ui {
		enum AnchorType {
			Center,
			TopLeft,
			Top,
			TopRight,
			Right,
			BottomRight,
			Bottom,
			BottomLeft,
			Left
		};

		const std::vector<String> ANCHOR_TYPES = {
			"Center",
			"Top Left",
			"Top",
			"Top Right",
			"Right",
			"Bottom Right",
			"Bottom",
			"Bottom Left",
			"Left"
		};

		// Calculate the adjusted position of a rect inside a container (relative to the container) for a given type and padding
		Vec2 calculateAnchoredPos(Rect container, Rect rect, AnchorType type, float padding = 0) {
			switch (type) {
			case AnchorType::Center:
				return (container.getCenter() - rect.size * 0.5f) - container.pos;
			case AnchorType::TopLeft:
				return Vec2(padding, container.size[1] - rect.size[1] - padding);
			case AnchorType::Top:
				return Vec2(container.size[0] * 0.5f - rect.size[0] * 0.5f, container.size[1] - rect.size[1] - padding);
			case AnchorType::TopRight:
				return Vec2(container.size[0] - rect.size[0] - padding, container.size[1] - rect.size[1] - padding);
			case AnchorType::Right:
				return Vec2(container.size[0] - rect.size[0] - padding, container.size[1] * 0.5f - rect.size[1] * 0.5f);
			case AnchorType::BottomLeft:
				return Vec2(padding, padding);
			case AnchorType::Bottom:
				return Vec2(container.size[0] * 0.5f - rect.size[0] * 0.5f, padding);
			case AnchorType::BottomRight:
				return Vec2(container.size[0] - rect.size[0] - padding, padding);
			case AnchorType::Left:
				return Vec2(padding, container.size[1] * 0.5f - rect.size[1] * 0.5f);
			}
		}
	}
}

#endif
