#ifndef EXR_RENDERER_H
#define EXR_RENDERER_H

#include "../ShaderProgram.h"
#include "../EXR.h"

namespace hagame {
	namespace graphics {
		struct EXRRenderer {
			RawPtr<ShaderProgram> shader;
			RawPtr<EXR> exr;
		};
	}
}

#endif