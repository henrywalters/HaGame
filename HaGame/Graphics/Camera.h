#ifndef CAMERA
#define CAMERA

#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Hypercube.h"
#include <gl/glew.h>
#include "ShaderProgram.h"
#include "../Utils/Aliases.h"
#include "../Core/Transform.h"

namespace hagame {
	namespace graphics {

		class Camera {
		public:

			Transform transform;

			virtual Mat4 getViewMatrix() = 0;
			virtual Mat4 getProjMatrix() = 0;

			GLint getMVPUniformLocation(ShaderProgram program) {
				return glGetUniformLocation(program.id, "MVP");
			}
		};
	}
}

#endif