#include "Transform.h"

hagame::Transform::Transform() {
	position = Vec3::Zero();
	scale = Vec3::Identity();
	rotation = Quat(0, Vec3::Top());
	updateModel();
}

void hagame::Transform::move(Vec3 offset)
{
	position += offset;
	updateModel();
}

void hagame::Transform::rotate(Vec3 axis, float degrees) {
	rotation += Quat(degrees, axis);
	updateModel();
}

void hagame::Transform::rotate(Quat rot) {
	rotation = rotation * rot;
	updateModel();
}

void hagame::Transform::grow(Vec3 size) {
	scale += size;
	updateModel();
}

void hagame::Transform::lookAt(Vec3 point)
{
	//Vec3 pos = position.normalized();
	//point.normalize();
	//float dot = clamp(pos.dot(point), -1.0f, 1.0f);
	//rotationAxis = hgVec3(glm::cross(glVec3(pos), glVec3(point)));
	//rotation = glm::acos(dot);
}

Mat4 hagame::Transform::getTranslationMatrix() {
	return Mat4::Translation(position);
}

Mat4 hagame::Transform::getScaleMatrix() {
	return Mat4::Scale(scale);
}

Mat4 hagame::Transform::getRotationMatrix() {
	return Mat4::Rotation(rotation);
}

Mat4 hagame::Transform::getModelMatrix() {
	return getTranslationMatrix()* getRotationMatrix() * getScaleMatrix();
}

Vec3 hagame::Transform::top()
{
	return rotation.rotatePoint(Vec3::Top());
}

Vec3 hagame::Transform::right()
{
	return rotation.rotatePoint(Vec3::Right());
}

Vec3 hagame::Transform::face()
{
	return rotation.rotatePoint(Vec3::Face());
}
