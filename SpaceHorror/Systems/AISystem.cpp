#include "AISystem.h"

void AISystem::onSystemUpdate(double dt)
{
	forEach<Walker>([this, dt](Walker* walker, RawPtr<hagame::ecs::Entity> entity) {
		walker->renderLineOfSight(entity->getPos());
	});
}

void AISystem::onSystemPhysicsBeforeUpdate(double dt)
{
	forEach<Walker>([this, dt](Walker* walker, RawPtr<hagame::ecs::Entity> entity) {
		walker->update(entity, &scene->ecs, &game->collisions, entity->getPos(), dt);
	});
}


void AISystem::onSystemPhysicsUpdate(double dt)
{
	forEach<Walker>([this, dt](Walker* walker, RawPtr<hagame::ecs::Entity> entity) {
		entity->getComponent<RigidBody>()->applyForce(walker->getMovementForce());
	});
}

