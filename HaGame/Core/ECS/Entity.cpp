#include "Entity.h"
#include "../Transform.h"

Vec3 hagame::ecs::Entity::getPos() {
	return transform->getPosition();
}

Quat hagame::ecs::Entity::getRot() {
	return transform->getRotation();
}

Vec3 hagame::ecs::Entity::getScale() {
	return transform->getScale();
}

void hagame::ecs::Entity::move(Vec3 delta)
{
	transform->move(delta);
}

void hagame::ecs::Entity::setPos(Vec3 pos)
{
	transform->setPosition(pos);
}

void hagame::ecs::Entity::setRot(Quat rot)
{
	transform->setRotation(rot);
}

void hagame::ecs::Entity::setScale(Vec3 scale)
{
	transform->setScale(scale);
}
