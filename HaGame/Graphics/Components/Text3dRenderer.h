#ifndef TEXT_3D_RENDERER_H
#define TEXT_3D_RENDERER_H

#include "./../Text.h"
#include "./../ShaderProgram.h"

namespace hagame {
	namespace graphics {
		struct Text3dRenderer {
			ShaderProgram* shader;
			Font* font;
			Color color = Color::red();
			String message = "";
			// if maxLength is a number greater than 0, each line of text will be that length at most.
			float maxLength = 0.0f;
		};
	}
}

#endif
