#ifndef SPRITE_2D_RENDERER_H
#define SPRITE_2D_RENDERER_H

#include "../Sprite2D.h"

namespace hagame {
	namespace graphics  {
		struct Sprite2DRenderer{
			Ptr<Sprite2D> sprite;
			ShaderProgram* shader;
		};
	}
}

#endif