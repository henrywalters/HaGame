#ifndef TRANSFORM
#define TRANSFORM

#include "../Math/Vector.h"
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Utils/Aliases.h"
#include "../Math/Functions.h"

namespace hagame {
	class Transform {
	public:
		Vec3 position;
		Vec3 scale;
		Vec3 rotationAxis;
		float rotation;

		Transform();

		void move(Vec3 offset);
		void rotate(Vec3 axis, float degrees);
		void grow(Vec3 size);

		void lookAt(Vec3 point);

		glm::mat4 getTranslationMatrix();
		glm::mat4 getScaleMatrix();
		glm::mat4 getRotationMatrix();

		glm::mat4 getModelMatrix();
	};
}

#endif