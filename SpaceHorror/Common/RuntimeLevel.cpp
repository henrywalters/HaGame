#include "RuntimeLevel.h"

using namespace hagame::input::devices;
using namespace hagame::graphics;
using namespace hagame::physics;
using namespace hagame::ui;

RawPtr<hagame::ecs::Entity> RuntimeLevel::addCamera()
{
	camera = addEntity();
	auto cameraComp = camera->addComponent<hagame::graphics::CameraComponent>();
	orth = std::make_shared<hagame::graphics::OrthographicCamera>(game->window->size / game->window->pixelsPerMeter);
	cameraComp->camera = orth;
	//camera->transform->setPosition(game->window->size.resize<3>() / 2.0f);
	return camera;
}

RawPtr<hagame::ecs::Entity> RuntimeLevel::addHUD()
{
	auto gridEntity = addEntity();
	auto grid = gridEntity->addComponent<Grid>(game->window->size, 6, 6);
	grid->display = false;

	auto ammoDisplay = addEntity();
	auto ammoText = ammoDisplay->addComponent<TextRenderer>();
	ammoText->shader = game->resources->getShaderProgram("text");
	ammoText->color = SECONDARY;
	ammoText->font = game->resources->getFont("secondary_small");
	ammoText->message = "0 / 0";

	grid->addEntity(5, 5, ammoDisplay, [](Entity* entity) { return entity->getComponent<TextRenderer>(); });
}

RawPtr<hagame::ecs::Entity> RuntimeLevel::addSprite(String textureName, Vec2 pos, Vec2 size)
{
	auto entity = addEntity();
	entity->addTag("resizable");
	auto renderer = entity->addComponent<hagame::graphics::Sprite2DRenderer>();
	renderer->shader = game->resources->getShaderProgram("sprite2d");
	renderer->sprite = std::make_shared<hagame::graphics::Sprite2D>();
	renderer->sprite->quad = std::make_shared<Quad>(size);
	renderer->sprite->texture = game->resources->getTexture(textureName);
	entity->transform->setPosition(pos.resize<3>());
	return entity;
}

RawPtr<hagame::ecs::Entity> RuntimeLevel::addLight(Vec2 pos, float radius, Color color)
{
	auto entity = addEntity();
	entity->addTag("resizable");
	auto renderer = entity->addComponent<hagame::graphics::DiscLightRenderer>();
	renderer->shader = game->resources->getShaderProgram("light2d");
	renderer->disc = std::make_shared<hagame::graphics::Disc>(radius);
	renderer->color = color;
	entity->transform->setPosition(pos.resize<3>());
	return entity;
}

RawPtr<hagame::ecs::Entity> RuntimeLevel::addEXR(String exrName, Vec2 pos, Vec2 size)
{
	auto entity = addEntity();
	auto renderer = entity->addComponent<hagame::graphics::EXRRenderer>();
	renderer->shader = game->resources->getShaderProgram("exr");
	renderer->exr = game->resources->getEXR(exrName);
	renderer->exr->setSize(size);
	entity->transform->setPosition(pos.resize<3>());
	return entity;
}

void RuntimeLevel::addPhysics(RawPtr<hagame::ecs::Entity> entity, float mass)
{
	auto rb = entity->addComponent<hagame::physics::RigidBody>();
	rb->mass = mass;

	auto platformer = entity->addComponent<Platformer>();

	// rb->forceDueToGravity = 0;
}

hagame::physics::Collider* RuntimeLevel::addBoxCollider(RawPtr<hagame::ecs::Entity> entity, Vec2 size, bool dynamic)
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

void RuntimeLevel::renderBlockOutline(Vec2 pos)
{
	hagame::graphics::drawRect(Rect(pos.rounded() * BLOCK_SIZE - BLOCK_SIZE * 0.5, BLOCK_SIZE), Color::green(), DEBUG_SHADER, 0.1f);
}

void RuntimeLevel::setMousePos(Vec2 rawMousePos)
{
	rawMousePos[1] = game->window->size[1] - rawMousePos[1];
	mousePos = orth->getGamePos(camera->transform, rawMousePos / game->window->pixelsPerMeter);
}

void RuntimeLevel::setWindowSize(Vec2 size)
{
	orth->size = size / game->window->pixelsPerMeter;
	uiOrth->size = size;
	cellSize = size / 10.0f;
}

bool RuntimeLevel::guiActive()
{
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
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

RawPtr<hagame::ecs::Entity> RuntimeLevel::addQuad(Vec3 pos, Vec2 size, Color color) {
	auto entity = addEntity();
	entity->move(pos);
	auto quad = entity->addComponent<QuadRenderer>(size);
	quad->color = color;
	quad->shader = game->resources->getShaderProgram("color");
	return entity;
}