#ifndef SPRITE_3D_RENDERER_H
#define SPRITE_3D_RENDERER_H

#include "../Sprite3D.h"

namespace hagame {
	namespace graphics  {
		struct Sprite3DRenderer{
			Ptr<Sprite3D> sprite;
			ShaderProgram* shader;
		};
	}
}

#endif