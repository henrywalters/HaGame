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

	private:

	public:
		Vec3 position;
		Vec3 scale;
		Quat rotation;

		Mat4 model;
		bool cacheModel = true;

		Transform();

		void move(Vec3 offset);
		void rotate(Vec3 axis, float degrees);
		void rotate(Quat rot);
		void grow(Vec3 size);

		void lookAt(Vec3 point);

		void updateModel() {
			if (cacheModel) model = getModelMatrix();
		}

		Mat4 getTranslationMatrix();
		Mat4 getScaleMatrix();
		Mat4 getRotationMatrix();

		Mat4 getModelMatrix();

		Vec3 top();
		Vec3 right();
		Vec3 face();
	};
}

#endif