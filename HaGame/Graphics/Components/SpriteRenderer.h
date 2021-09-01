#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "../Sprite.h"

namespace hagame {
	namespace graphics {
		struct SpriteRenderer {
			Ptr<Sprite> sprite;
			ShaderProgram* shader;
		};
	}
}

#endif