#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "./../Font.h"
#include "./../Color.h"
#include "./../ShaderProgram.h"

namespace hagame {
	namespace graphics {
		struct TextRenderer {
			ShaderProgram* shader;
			Font* font;
			Vec2 pos;
			Color color = Color::red();
			String message = "";
			// if maxLength is a number greater than 0, each line of text will be that length at most.
			float maxLength = 0.0f;
		};
	}
}

#endif
