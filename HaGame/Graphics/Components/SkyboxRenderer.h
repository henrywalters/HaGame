#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include "../Cubemap.h"
#include "../ShaderProgram.h"

namespace hagame {
	namespace graphics {
		struct SkyboxRenderer {
			Cubemap* cubemap;
			ShaderProgram* shader;
		};
	}
}

#endif
