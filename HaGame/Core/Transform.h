#ifndef TRANSFORM
#define TRANSFORM

#include "../Math/Vector.h"
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Utils/Aliases.h"
#include "../Math/Functions.h"
#include "ECS/Entity.h"

namespace hagame {
	class Transform {

	private:
		Vec3 position;
		Vec3 scale;
		Quat rotation;
	public:


		Mat4 model;
		Mat4 modelInverse;
		bool cacheModel = true;

		ecs::Entity* entity;

		Transform();

		Vec3 getPosition();
		Vec3* getPositionRef();
		Quat getRotation();
		Vec3 getScale();

		void setPosition(Vec3 pos);
		void setRotation(Quat rot);
		void setScale(Vec3 size);

		void move(Vec3 offset);
		void rotate(Vec3 axis, float degrees);
		void rotate(Quat rot);
		void rotateAbout(Vec3 point, Quat rot);
		bool lockRotation = false;
		void grow(Vec3 size);

		void lookAt(Vec3 point);

		void updateModel() {
			model = getModelMatrix();
			modelInverse = model.inverted();
		}

		Mat4 getTranslationMatrix();
		Mat4 getScaleMatrix();
		Mat4 getRotationMatrix();

		Mat4 getModelMatrix();

		Vec3 top();
		Vec3 bottom();
		Vec3 right();
		Vec3 left();
		Vec3 face();
		Vec3 back();
	};
}

#endif