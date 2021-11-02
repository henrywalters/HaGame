#include "Transform.h"

hagame::Transform::Transform() {
	position = Vec3::Zero();
	scale = Vec3::Identity();
	rotation = Quat(0.0f, Vec3::Top());
	// updateModel();
}

Vec3 hagame::Transform::getPosition()
{
	if (entity != NULL && entity->parent != NULL)
		return entity->parent->transform->getPosition() + position;
	else
		return position;
}

Vec3* hagame::Transform::getPositionRef()
{
	return &position;
}

Quat hagame::Transform::getRotation()
{
	if (entity != NULL && entity->parent != NULL)
		return entity->parent->transform->getRotation() * rotation;
	else
		return rotation;
}

Vec3 hagame::Transform::getScale()
{
	if (entity != NULL && entity->parent != NULL)
		return entity->parent->transform->getScale() + scale;
	else
		return scale;
}

void hagame::Transform::setPosition(Vec3 pos)
{
	auto delta = (position - pos) * -1;
	position = pos;
	updateModel();
	/*if (entity) {
		for (auto child : entity->children) {
			child->transform->move(delta);
		}
	}*/
}

void hagame::Transform::setRotation(Quat rot)
{
	if (!lockRotation) {
		Quat delta = rot * rotation.inverse();
		rotation = rot;
		updateModel();
		/*if (entity) {
			for (auto child : entity->children) {
				if (!child->transform->lockRotation) {
					child->transform->position = delta.rotatePoint(child->transform->position - position) + position;
					child->transform->rotate(delta);
				}
			}
		}*/
	}
}

void hagame::Transform::setScale(Vec3 size)
{
	Vec3 offset = size.div(scale);
	scale = size;
	updateModel();
	/*if (entity) {
		for (auto child : entity->children) {
			child->transform->grow(offset);
		}
	}*/
}

void hagame::Transform::move(Vec3 offset)
{
	position += offset;
	updateModel();
	/*if (entity) {
		for (auto child : entity->children) {
			child->transform->move(offset);
		}
	}*/
	
}

void hagame::Transform::rotate(Vec3 axis, float degrees) {
	if (!lockRotation) {
		rotation = rotation * Quat(degrees, axis);
		updateModel();
	}
}

void hagame::Transform::rotate(Quat rot) {
	if (!lockRotation) {
		rotation = rotation * rot;
		updateModel();
		/*if (entity) {
			for (auto child : entity->children) {
				if (!child->transform->lockRotation) {
					child->transform->position = rot.rotatePoint(child->transform->position - position) + position;
					child->transform->rotate(rot);
				}
			}
		}*/
	}
}


void hagame::Transform::grow(Vec3 size) {
	scale += size;
	updateModel();
}

void hagame::Transform::lookAt(Vec3 point)
{
	Vec3 forward = normalize(point - getPosition());
	float d = dot(forward, Vec3::Face());
	float rot = acos(d);
	Vec3 axis = normalize(cross(Vec3::Face(), forward));
	rotation = Quat(rot, axis);
}

Mat4 hagame::Transform::getTranslationMatrix() {
	return Mat4::Translation(getPosition());
}

Mat4 hagame::Transform::getScaleMatrix() {
	return Mat4::Scale(getScale());
}

Mat4 hagame::Transform::getRotationMatrix() {
	return Mat4::Rotation(getRotation());
}

Mat4 hagame::Transform::getModelMatrix() {
	return getTranslationMatrix()* getRotationMatrix() * getScaleMatrix();
}

Vec3 hagame::Transform::top()
{
	return getRotation().rotatePoint(Vec3::Top());
}

Vec3 hagame::Transform::bottom()
{
	return getRotation().rotatePoint(Vec3::Bottom());
}

Vec3 hagame::Transform::right()
{
	return getRotation().rotatePoint(Vec3::Right());
}

Vec3 hagame::Transform::left ()
{
	return getRotation().rotatePoint(Vec3::Left());
}

Vec3 hagame::Transform::face()
{
	return getRotation().rotatePoint(Vec3::Face());
}

Vec3 hagame::Transform::back()
{
	return getRotation().rotatePoint(Vec3::Back());
}