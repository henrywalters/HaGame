#ifndef BOUNDING_BOX_RENDERER_H
#define BOUNDING_BOX_RENDERER_H

#include "../Color.h"
#include "../ShaderProgram.h"
#include "../../Utils/Aliases.h"

namespace hagame {
	namespace graphics {
		struct BoundingBoxRenderer {
			Cube boundingBox = Cube(Vec3::Identity(), Vec3::Zero());
			Color color = Color::blue();
			ShaderProgram* shader;
		};
	}
}

#endif