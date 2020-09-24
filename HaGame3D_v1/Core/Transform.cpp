#include "Transform.h"

hagame::Transform::Transform() {
	position = Vec3::Zero();
	scale = Vec3::Identity();
	rotation = Vec3::Identity();
}

void hagame::Transform::move(Vec3 offset)
{
	position += offset;
}

void hagame::Transform::rotate(Vec3 axis, float degrees) {
	// TODO: Fix this lol
}

void hagame::Transform::grow(Vec3 size) {
	scale += size;
}

glm::mat4 hagame::Transform::getTranslationMatrix() {
	return glm::translate(glm::mat4(1.0f), glVec3(position));
}

glm::mat4 hagame::Transform::getScaleMatrix() {
	return glm::scale(glm::mat4(1.0f), glVec3(scale));
}

glm::mat4 hagame::Transform::getRotationMatrix() {
	return glm::mat4(1.0f);
}

glm::mat4 hagame::Transform::getModelMatrix() {
	return getTranslationMatrix()* getRotationMatrix()* getScaleMatrix();
}