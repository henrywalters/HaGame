#ifndef IMSIM_LEVEL_EDITOR_HPP
#define IMSIM_LEVEL_EDITOR_HPP

#include "./../../HaGame/HaGame.h"
#include "../Systems/LevelEditorSystem.hpp"

using namespace hagame::graphics;
using namespace hagame::ecs;

class LevelEditor : public hagame::Scene {
public:
	
	Ptr<Entity> light;
	Ptr<Entity> cameraEntity;

	void onSceneBeforeActivate() {
		loadResources();

		addSystem<ImSimMovementSystem>();
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		addSystem<TestSystem>();
		addSystem<CollisionSystem>();
		addSystem<LevelEditorSystem>();

		cameraEntity = addEntity();
		auto camera = cameraEntity->addComponent<CameraComponent>();
		auto perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;

		cameraEntity->transform->move(Vec3::Top(2.0f));

		addLight(Vec3::Top(10.0f));
		addFloor();
	}

	void onSceneUpdate(double dt) {
		
	}

	Ptr<Entity> addFloor() {
		auto floor = addEntity();
		auto collider = floor->addComponent<Collider>();
		collider->boundingCube = Cube(Vec3({ -(float)GRID_COLS / 2.0f, 0.0f, -(float)GRID_COLS / 2.0f }), Vec3({ (float)GRID_COLS, 0.0f, (float)GRID_ROWS }));
		return floor;
	}

	void loadResources() {

		game->resources->loadMesh("cube", "Models/Cube.obj");
		game->resources->loadMesh("triangle", Triangle(Vec3({ 0, 0, 0 }), Vec3({ 0, 1, 0 }), Vec3({ 1, 0, 0 })).getMesh());

		game->resources->loadFont("arial", "Fonts/arial.ttf");

		// Textures
		game->resources->loadTexture("crate", "Textures/crate.jpg");
		game->resources->loadTexture("proto", "Textures/prototype.jpg");
		game->resources->loadTexture("chain_base", "Textures/chainwall/basecolor.png");
		game->resources->loadTexture("chain_normal", "Textures/chainwall/normal.png");
		game->resources->loadTexture("chain_opacity", "Textures/chainwall/opacity.png");

		Array<String> shaders = {
			"color",
			"texture",
			"text",
			"material"
		};

		for (auto shader : shaders) {
			game->resources->loadShaderProgram(shader, "Shaders/" + shader + "_vert.glsl", "Shaders/" + shader + "_frag.glsl");
		}
	}

	void addLight(Vec3 pos) {
		light = addEntity();
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
	}
};

#endif
