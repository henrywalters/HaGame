#include "RuntimeLevel.h"

Ptr<hagame::ecs::Entity> RuntimeLevel::addCamera()
{
	camera = addEntity();
	auto cameraComp = camera->addComponent<hagame::graphics::CameraComponent>();
	orth = std::make_shared<hagame::graphics::OrthographicCamera>(game->window->size / PIXELS_PER_METER);
	cameraComp->camera = orth;
	//camera->transform->setPosition(game->window->size.resize<3>() / 2.0f);
	return camera;
}

Ptr<hagame::ecs::Entity> RuntimeLevel::addSprite(String textureName, Vec2 pos, Vec2 size)
{
	Ptr<hagame::ecs::Entity> entity = addEntity();
	entity->addTag("resizable");
	auto renderer = entity->addComponent<hagame::graphics::SpriteRenderer>();
	renderer->shader = game->resources->getShaderProgram("sprite");
	renderer->sprite = std::make_shared<hagame::graphics::Sprite>(
		game->resources->getTexture(textureName),
		Rect(Vec2::Zero(), cellSize.prod(size))
	);
	entity->transform->setPosition(pos.resize<3>());
	return entity;
}

void RuntimeLevel::addPhysics(Ptr<hagame::ecs::Entity> entity, float mass)
{
	auto rb = entity->addComponent<hagame::physics::RigidBody>();
	rb->mass = mass;
	rb->forceDueToGravity = -9.8;

	auto platformer = entity->addComponent<Platformer>();

	// rb->forceDueToGravity = 0;
}

hagame::physics::Collider* RuntimeLevel::addBoxCollider(Ptr<hagame::ecs::Entity> entity, Vec2 size, bool dynamic)
{
	auto collider = entity->addComponent<hagame::physics::Collider>();
	auto boundingVolume = entity->addComponent<hagame::physics::BoundingVolume>(Cube(Vec2::Zero(), size));
	boundingVolume->dynamic = dynamic;
	collider->dynamic = dynamic;
	collider->type = hagame::physics::ColliderType::BoxCollider;
	collider->boundingCube = Cube(size * -0.5f,  size);
	collider->boundingVolume = boundingVolume;
	return collider;
}

hagame::physics::Collider* RuntimeLevel::addCircleCollider(Ptr<hagame::ecs::Entity> entity, float radius, bool dynamic)
{
	auto collider = entity->addComponent<hagame::physics::Collider>();
	auto boundingVolume = entity->addComponent<hagame::physics::BoundingVolume>(Sphere(Vec2::Zero(), radius));
	boundingVolume->dynamic = dynamic;
	collider->type = hagame::physics::ColliderType::SphereCollider;
	collider->boundingSphere = Sphere(Vec3::Zero(), radius);
	collider->boundingVolume = boundingVolume;
	return collider;
}

void RuntimeLevel::setMousePos(Vec2 rawMousePos)
{
	rawMousePos[1] = game->window->size[1] - rawMousePos[1];
	mousePos = orth->getGamePos(camera->transform, rawMousePos / PIXELS_PER_METER);
}

void RuntimeLevel::setWindowSize(Vec2 size)
{
	orth->size = size / PIXELS_PER_METER;
	//camera->transform->setPosition(game->window->size.resize<3>() / 2.0f);
	cellSize = size / 10.0f;
	for (auto entityId : ecs.getRegistry()->view<hagame::graphics::SpriteRenderer>()) {
		auto entity = ecs.entities.getByEnttId(entityId);
		if (entity->hasTag("resizable")) {
			std::cout << cellSize << "\n";
			entity->getComponent<hagame::graphics::SpriteRenderer>()->sprite->rect.size = cellSize;
		}
	}
}

float RuntimeLevel::binarySearchCollisionTime(Cube entityCube, Cube otherCube, Vec2 velocity, double dt)
{
	float minT = 0;
	float maxT = dt;
	float midT = 0;

	while (abs(maxT - minT) > 0.01) {
		midT = (maxT - minT) / 2.0f;
		auto movedCube = entityCube;
		movedCube.pos += velocity * midT;
		auto hit = hagame::math::collisions::checkAABBAgainstAABB(hagame::math::AABB(movedCube), hagame::math::AABB(otherCube));
		if (hit.has_value()) {
			maxT = midT;
		}
		else {
			minT = midT;
		}
	}
	return midT - 0.01;
}
