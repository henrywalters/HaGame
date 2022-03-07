#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "./../Font.h"
#include "./../Color.h"
#include "./../ShaderProgram.h"
#include "./../../UI/Components/UIComponent.h"

namespace hagame {
	namespace graphics {
		struct TextRenderer : public hagame::ui::UIComponent {
			ShaderProgram* shader;
			Font* font;
			int fontSize = 24;
			Color color = Color::red();
			String message = "";
			// if maxLength is a number greater than 0, each line of text will be that length at most.
			float maxLength = 0.0f;
			TextHAlignment alignmentH = TextHAlignment::Center;
			TextVAlignment alignmentV = TextVAlignment::Center;

			Rect getBoundingBox() {
				return Rect(Vec2::Zero(), font->calculateMessageSize(message));
			}
		};
	}
}

#endif
