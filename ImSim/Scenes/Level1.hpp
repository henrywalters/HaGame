#ifndef IMSIM_LEVEL_1_HPP
#define IMSIM_LEVEL_1_HPP

#include "../../HaGame/HaGame.h"
#include "../Systems/MouseSystem.hpp"
#include "../Systems/StateSystem.hpp"
#include "../Systems/Movement.hpp"
#include "../Components/PlayerMovement.hpp"

using namespace hagame::ecs;
using namespace hagame::graphics;

class Level1 : public hagame::Scene {
public:
	Ptr<Entity> player;
	Ptr<Entity> gun;
	Ptr<Entity> light;
	Ptr<Entity> cameraEntity;
	Ptr<Entity> debugCameraEntity;
	Ptr<MouseSystem> mouse;
	Ptr<Entity> redDot;

	Ptr<CameraSystem> cameraSystem;

	void onSceneBeforeActivate() {
		loadResources();

		addSystem<ImSimMovementSystem>();
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		cameraSystem = addSystem<CameraSystem>();
		addSystem<TestSystem>();
		addSystem<CollisionSystem>();
		mouse = addSystem<MouseSystem>();
		auto stateSystem = addSystem<StateSystem>();

		player = addEntity();
		player->transform->move(Vec3::Top(2.5f));
		
		player->addComponent<PlayerMovement>();

		auto pCollider = player->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 1.5f);


		auto body = addChild(player);
		addCylinder(body, 0.25f, 1.0f);
		cameraEntity = addChild(player);
		auto camera = cameraEntity->addComponent<CameraComponent>();
		auto perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;
		

		//gun = addChild(player);
		//gun->transform->move(Vec3({ -1.0f, 0.0f, 0.0f }));
		//auto meshRenderer = gun->addComponent<MeshRenderer>();
		//meshRenderer->mesh = game->resources->getMesh("glock");
		//meshRenderer->texture = game->resources->getTexture("proto");
		//meshRenderer->shader = game->resources->getShaderProgram("texture");
		//meshRenderer->color = Color::white();

		auto fpsCam = player->addComponent<FPSCameraController>();
		fpsCam->camera = perspectiveCam.get();
		fpsCam->inputFn = [this]() {
			FPSCameraControllerParams params;
			params.look = game->input.keyboardMouse.rAxis;
			params.zoomed = game->input.keyboardMouse.keyboard.lCtrl;
			return params;
		};

		cameraEntity->transform->move(Vec3::Top(2.0f));

		debugCameraEntity = addEntity();
		debugCameraEntity->transform->move(Vec3({ 5, 5, 5 }));
		debugCameraEntity->transform->lookAt(player->transform->getPosition());
		debugCameraEntity->addComponent<DebugCamera>();
		camera = debugCameraEntity->addComponent<CameraComponent>();
		perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;
		camera->active = false;

		auto meshRenderer = debugCameraEntity->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("camera");
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();

		addLight(Vec3::Top(10.0f));
		addFloor();

		addCube(Vec3::Zero(), Vec3({ 100, 0.1f, 100 }));
		addCube(Vec3({ -0.5, 0.5, 5 }), Vec3(1.0f));
		addCube(Vec3({ 0.5, 0.5, 5 }), Vec3(1.0f));
		addCube(Vec3({ 0, 1.4f, 5 }), Vec3(0.8f));

		redDot = addEntity();
		auto dotRenderer = redDot->addComponent<DynamicMeshRenderer>();
		dotRenderer->mesh = std::make_shared<Quad>(0.03f, 0.03f);
		dotRenderer->texture = game->resources->getTexture("red_dot");
		dotRenderer->shader = game->resources->getShaderProgram("texture");
		dotRenderer->color = Color::white();

		stateSystem->events->subscribe(StateEvents::DebugChange, [this](Ptr<State> state) {
			if (state->debug) {
				cameraSystem->active = false;
				debugCameraEntity->getComponent<CameraComponent>()->active = true;
				cameraEntity->getComponent<CameraComponent>()->active = false;
			}
			else {
				cameraSystem->active = true;
				debugCameraEntity->getComponent<CameraComponent>()->active = false;
				cameraEntity->getComponent<CameraComponent>()->active = true;
			}
		});

		stateSystem->events->subscribe(StateEvents::LockMouseChange, [this](Ptr<State> state) {
			if (state->lockMouse) {
				game->input.keyboardMouse.captureMouseOn();
				game->input.keyboardMouse.hideCursor();
			}
			else {
				game->input.keyboardMouse.captureMouseOff();
				game->input.keyboardMouse.showCursor();
			}
		});

		stateSystem->events->subscribe(StateEvents::VSyncChange, [this](Ptr<State> state) {
			if (state->vsync) {
				game->window->turnOnVSync();
			}
			else {
				game->window->turnOffVSync();
			}
		});
	}

	void onSceneUpdate(double dt) {

		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), viewMat, projMat);

		ImGui::Begin("Stats");

		if (mouse->rayHit.has_value()) {
			auto rotation = mouse->rayHit.value().normal.rotationBetween(Vec3::Face());
			redDot->transform->setPosition(mouse->rayHit.value().point + mouse->rayHit.value().normal * 0.001f);
			redDot->transform->setRotation(rotation);

			ImGui::Text((player->transform->getPosition().toString().c_str()));
			ImGui::Text((player->transform->right().toString().c_str()));
			ImGui::Text((activeCameraEntity->transform->getPosition() + player->transform->right()).toString().c_str());

			// drawLine(Line(gun->transform->getPosition(), mouse->rayHit.value().point, Color::red(), 5), colorShader);
		}
		else {
			redDot->transform->setPosition(Vec3(-10000.0f));
		}

		//drawSphereOutline(player->transform->getPosition(), 1.5f, Color::blue(), colorShader);

		ImGui::End();
	}

	Ptr<Entity> addCube(Vec3 pos, Vec3 size) {
		auto cube = addEntity();
		auto meshRenderer = cube->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<RectPrism>(size);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		auto collider = cube->addComponent<Collider>();
		collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();

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
		auto collider = entity->addComponent<Collider>();
		collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
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
		game->resources->loadMesh("camera", "Models/camera.obj");
		game->resources->loadMesh("glock", "Models/glock.obj");

		game->resources->loadFont("arial", "Fonts/arial.ttf");

		// Textures
		game->resources->loadTexture("crate", "Textures/crate.jpg");
		game->resources->loadTexture("proto", "Textures/prototype.jpg");
		game->resources->loadTexture("chain_base", "Textures/chainwall/basecolor.png");
		game->resources->loadTexture("chain_normal", "Textures/chainwall/normal.png");
		game->resources->loadTexture("chain_opacity", "Textures/chainwall/opacity.png");
		game->resources->loadTexture("red_dot", "Textures/red_dot.png");

		Array<String> shaders = {
			"color",
			"texture",
			"text",
			"material",
			"sprite"
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
