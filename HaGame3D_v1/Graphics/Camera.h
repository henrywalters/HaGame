#include "../Math/Vector.h"
#include <gl/glew.h>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "../Utils/Aliases.h"

#ifndef CAMERA
#define CAMERA

namespace hagame {
	namespace graphics {
		class Camera {
		public:
			float xRot;
			float yRot;

			Vec3 position;
			Vec3 direction;
			Vec3UInt32 size;
			float fov;
			float zNear;
			float zFar;

			Camera(Vec3UInt32 _size);

			glm::mat4 getOrthographicMatrix();
			glm::mat4 getProjectionMatrix();
			glm::mat4 getViewMatrix();

			GLint getMVPUniformLocation(ShaderProgram program);
		};
	}
}

#endif