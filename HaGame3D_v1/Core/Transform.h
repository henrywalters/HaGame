#ifndef TRANSFORM
#define TRANSFORM

#include "../Math/Vector.h"
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Utils/Aliases.h"

namespace hagame {
	class Transform {
	public:
		Vec3 position;
		Vec3 scale;
		Vec3 rotation;

		Transform();

		void move(Vec3 offset);
		void rotate(Vec3 axis, float degrees);
		void grow(Vec3 size);

		glm::mat4 getTranslationMatrix();
		glm::mat4 getScaleMatrix();
		glm::mat4 getRotationMatrix();

		glm::mat4 getModelMatrix();
	};
}

#endif