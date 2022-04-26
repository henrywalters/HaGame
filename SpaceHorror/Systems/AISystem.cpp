#include "AISystem.h"

void AISystem::onSystemUpdate(double dt)
{
	
}

void AISystem::onSystemPhysicsUpdate(double dt)
{
	forEach<Walker>([this, dt](Walker* walker, RawPtr<hagame::ecs::Entity> entity) {
		walker->update(entity, &scene->ecs, &game->collisions, entity->getPos(), dt);
		entity->getComponent<RigidBody>()->applyForce(walker->getMovementForce());
	});
}
