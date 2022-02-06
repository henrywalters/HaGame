#ifndef IMSIM_COMMON_HPP
#define IMSIM_COMMON_HPP

#include "./../../HaGame/HaGame.h"

using namespace hagame::ecs;
using namespace hagame::graphics;

struct RuntimeLevel : public hagame::Scene {

	const Vec3 CELL_SIZE = Vec3(1.0f);

	Ptr<Entity> addSprite2D(Vec2 pos, Vec2 size, String textureName) {
		auto entity = addEntity();
		entity->transform->setPosition(pos.resize<3>(1.0f));
		auto mesh = entity->addComponent<Sprite2DRenderer>();
		mesh->sprite = std::make_shared<Sprite2D>();
		mesh->sprite->quad = std::make_shared<Quad>(size);
		mesh->sprite->texture = game->resources->getTexture(textureName);
		mesh->shader = game->resources->getShaderProgram("sprite2d");
		return entity;
	}

	Ptr<Entity> addSprite3D(Vec3 pos, Vec2 size, String textureName) {
		auto entity = addEntity();
		entity->transform->setPosition(pos);
		auto mesh = entity->addComponent<Sprite3DRenderer>();
		mesh->sprite = std::make_shared<Sprite3D>();
		mesh->sprite->quad = std::make_shared<Quad>(size);
		mesh->sprite->texture = game->resources->getTexture(textureName);
		mesh->shader = game->resources->getShaderProgram("sprite3d");
		return entity;
	}

	Ptr<Entity> addCacodemon(Vec3 pos) {
		return addSprite3D(pos, Vec2(1.5f), "cacodemon");
	}

	Ptr<Entity> addRareRabbit(Vec3 pos) {
		return addSprite3D(pos, Vec2(1.5f), "rabbit");
	}

	Ptr<Entity> addMesh(Vec3 pos, Vec3 size, String meshName, String textureName) {
		auto entity = addEntity();
		auto mesh = entity->addComponent<MeshRenderer>();
		mesh->mesh = game->resources->getMesh(meshName);
		mesh->texture = game->resources->getTexture(textureName);
		mesh->shader = game->resources->getShaderProgram("texture");
		mesh->color = Color::white();
		mesh->material = Material::greenRubber();
		
		entity->transform->setScale(size);
		entity->transform->setPosition(pos);

		return entity;
	}

	Ptr<Entity> addCube(Vec3 pos, Vec3 size) {
		auto cube = addEntity();
		auto meshRenderer = cube->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<RectPrism>(size);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		auto collider = cube->addComponent<hagame::physics::Collider>();
		collider->boundingCube = Cube(size * -0.5f, size);
		auto partition = cube->addComponent<hagame::physics::BoundingVolume>(collider->boundingCube.value());
		
		//collider->boundingVolume = hagame::physics::BoundingVolume(&Sphere(pos, size.magnitude()));

		cube->transform->move(pos + CELL_SIZE * 0.5f);

		return cube;
	}

	void addCylinder(Ptr<Entity> entity, float radius, float height) {
		auto meshRenderer = entity->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<Cylinder>(radius, height);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		auto collider = entity->addComponent<hagame::physics::Collider>();
		collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
	}

	void addCone(Vec3 pos, float radius, float height) {
		auto entity = addEntity();
		entity->transform->setPosition(pos);
		auto meshRenderer = entity->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<Cone>(radius, height);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		//auto collider = entity->addComponent<Collider>();
		//collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
	}

	Ptr<Entity> addRamp(Vec3 pos, Vec3 size) {
		auto entity = addEntity();
		entity->transform->setPosition(pos);
		auto meshRenderer = entity->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<TriangularPrism>(size);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		//auto collider = entity->addComponent<Collider>();
		//collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
		return entity;
	}

	Ptr<Entity> addStairs(Vec3 pos, Vec3 size) {
		auto entity = addEntity();
		entity->transform->setPosition(pos);
		auto meshRenderer = entity->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<Stairs>(size, 5);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		//auto collider = entity->addComponent<Collider>();
		//collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
		return entity;
	}

	Ptr<Entity> addDisc(Vec3 pos, float radius) {
		auto entity = addEntity();
		entity->transform->setPosition(pos);
		auto meshRenderer = entity->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<Disc>(radius);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		//auto collider = entity->addComponent<Collider>();
		//collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
		return entity;
	}

	template <size_t M, size_t N>
	Ptr<Entity> addFloor(Vec3 pos, Vec2 size, bool random = false) {
		auto floor = addEntity();
		floor->transform->setPosition(pos);
		auto meshRenderer = floor->addComponent<DynamicMeshRenderer>();
		Ptr<Plane<M, N>> plane = std::make_shared<Plane<M, N>>(Plane<M, N>(size));
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();

		if (random) {
			hagame::math::Perlin<M, N, float> perlin = hagame::math::Perlin<M, N, float>();
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					auto idx = Vec2Int(i, j);
					auto point = plane->getPoint(idx);
					std::cout << point.xz().toString() << " -> ";
					point[1] = perlin.compute(point.xz().div(size / 3.0f));
					std::cout << point[1] << "\n";
					plane->setPoint(idx, point * 3.0f);
				}
			}
			plane->computeMesh();
		}
		meshRenderer->mesh = plane;
		//auto collider = floor->addComponent<Collider>();
		// collider->boundingCube = Cube(Vec3({ -(float)GRID_COLS / 2.0f, 1.0f, -(float)GRID_COLS / 2.0f }), Vec3({ (float)GRID_COLS, 0.0f, (float)GRID_ROWS }));
		
		return floor;
	}

	Ptr<Entity> addLight(Vec3 pos) {
		auto light = addEntity();
		light->transform->setScale(Vec3(0.5f));
		light->transform->setPosition(pos);
		auto lightC = light->addComponent<hagame::graphics::PointLightComponent>();
		lightC->light.ambient = Vec3({ 0.4, 0.4, 0.4 });
		lightC->light.diffuse = Vec3({ 0.5, 0.5, 0.5 });
		lightC->light.specular = Vec3({ 0.7, 0.7, 0.7 });

		auto meshRenderer = light->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("cube");
		meshRenderer->shader = game->resources->getShaderProgram("color");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		return light;
	}

};

#endif
