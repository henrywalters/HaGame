#ifndef RIGID_BODY_RENDERER_H
#define RIGID_BODY_RENDERER_H

#include "../Color.h"
#include "../ShaderProgram.h"

namespace hagame {
	namespace graphics {
		struct RigidBodyRenderer {
			Color velColor = Color::blue();
			float velLen = 1.0f;

			Color accelColor = Color::green();
			float accelLen = 1.0f;

			ShaderProgram* shader;
		};
	}
}

#endif