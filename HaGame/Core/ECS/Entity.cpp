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