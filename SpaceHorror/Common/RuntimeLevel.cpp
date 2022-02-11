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

void RuntimeLevel::resolveCollisions(Ptr<hagame::ecs::Entity> entity, Vec3& velocity, bool& grounded)
{

	if (velocity[0] == 0.0f && velocity[1] == 0.0f) return;

	auto collider = entity->getComponent<hagame::physics::Collider>();
	auto cube = collider->boundingCube.value();
	cube.pos += entity->getPos();
	auto ray = hagame::math::Ray(entity->getPos(), velocity );
	float velMag = velocity.magnitude();
	Cube groundCheck = Cube(cube.pos, Vec3(cube.size[0], cube.size[1] * 0.1f, 0));

	float t;

	Array<EntityHit> hits;

	//std::cout << velocity << "\n";

	hagame::graphics::drawCubeOutline(groundCheck, hagame::graphics::Color::green());

	//hagame::graphics::drawLine(ray.toLine(), hagame::graphics::Color::red());

	ecs.entities.forEach<hagame::physics::Collider>([&hits, &t, velMag, ray, entity, cube, groundCheck, &grounded](hagame::physics::Collider* nCollider, Ptr<hagame::ecs::Entity> neighbor) {
		if (neighbor == entity) return;

		auto nCube = nCollider->boundingCube.value();
		nCube.pos += neighbor->getPos() - (cube.size / 2.0f);
		nCube.size += cube.size;

		//hagame::graphics::drawCubeOutline(nCube, hagame::graphics::Color::red());

		auto hit = hagame::math::collisions::checkRayAgainstRect(ray, Rect(nCube.pos, nCube.size), t);

		if (hit.has_value() && t < 1.0f) {
			//std::cout << t << "\n";
			//hagame::graphics::drawCircle2D(hit.value().position, 0.2f, hagame::graphics::Color::red());
			hits.push_back(EntityHit{ neighbor, hit.value().normal, t });
			auto groundHit = hagame::math::collisions::checkAABBAgainstAABB(hagame::math::AABB(groundCheck), hagame::math::AABB(nCube));
			if (groundHit.has_value()) {
				grounded = true;
			}
		}
	});

	std::sort(hits.begin(), hits.end(), [](const EntityHit a, const EntityHit b) {
		return a.t < b.t;
	});

	//std::cout << "CHECKING " << hits.size() << " hits\n";

	//std::cout << velocity << "\n";

	for (auto hit : hits) {
		// std::cout << hit.normal * velMag * (1 - hit.t) << "\n";
		velocity += hit.normal * Vec2(abs(velocity[0]), abs(velocity[1])) * (1 - hit.t);
	}

	//std::cout << velocity << "\n";
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
