#ifndef CAMERA
#define CAMERA

#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Hypercube.h"
#include <gl/glew.h>
#include "ShaderProgram.h"
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {

		class Camera {
		public:
			
			const Cube DEFAULT_PROJ_SPACE = Cube(Vec3({ -5.0, -5.0, 0.1f }), Vec3({ 10.0f, 10.0f, 100.f }));

			Cube projectionSpace;

			Vec3 position;
			Quat rotation;

			Camera(Vec3 _size) : projectionSpace(Cube(Vec3({ 0, 0, 0 }), _size)), rotation(Quat(0, Vec3::Top())) {}
			Camera() : projectionSpace(DEFAULT_PROJ_SPACE), rotation(Quat(0, Vec3::Top())) {}

			Mat4 getOrthographicMatrix();
			Mat4 getPerspectiveMatrix();
			Mat4 getViewMatrix();

			GLint getMVPUniformLocation(ShaderProgram program);
		};
	}
}

#endif