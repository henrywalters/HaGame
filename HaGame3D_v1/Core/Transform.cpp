#include "Transform.h"

hagame::Transform::Transform() {
	position = Vec3::Zero();
	scale = Vec3::Identity();
	rotationAxis = Vec3::Identity();
	rotation = 0;
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

void hagame::Transform::lookAt(Vec3 point)
{
	Vec3 pos = position.normalized();
	point.normalize();
	float dot = clamp(pos.dot(point), -1.0f, 1.0f);
	rotationAxis = hgVec3(glm::cross(glVec3(pos), glVec3(point)));
	rotation = glm::acos(dot);
}

glm::mat4 hagame::Transform::getTranslationMatrix() {
	return glm::translate(glm::mat4(1.0f), glVec3(position));
}

glm::mat4 hagame::Transform::getScaleMatrix() {
	return glm::scale(glm::mat4(1.0f), glVec3(scale));
}

glm::mat4 hagame::Transform::getRotationMatrix() {
	return glm::rotate(glm::mat4(1.0), rotation, glVec3(rotationAxis));
}

glm::mat4 hagame::Transform::getModelMatrix() {
	return getTranslationMatrix()* getRotationMatrix()* getScaleMatrix();
}