#ifndef IMSIM_LEVEL_1_HPP
#define IMSIM_LEVEL_1_HPP

#include "../../HaGame/HaGame.h"
#include "../Systems/MouseSystem.hpp"
#include "../Systems/StateSystem.hpp"
#include "../Systems/Movement.hpp"
#include "../Systems/ProjectileSystem.hpp"
#include "../Systems/GizmoSystem.hpp"
#include "../Components/PlayerMovement.hpp"

using namespace hagame::ecs;
using namespace hagame::graphics;

struct Player {
	Ptr<Entity> player;
	Ptr<Entity> body;
	Ptr<Entity> broadCollider;
	Ptr<Entity> groundCollider;
	Ptr<Entity> camera;
	Ptr<Entity> gun;
};

class Level1 : public hagame::Scene {
public:
	Player player;
	Ptr<Entity> gun;
	Ptr<Entity> light;
	Ptr<Entity> debugCameraEntity;
	Ptr<MouseSystem> mouse;
	Ptr<Entity> redDot;
	Ptr<Quad> projQuad;
	Ptr<CameraSystem> cameraSystem;
	Ptr<StateSystem> stateSystem;
	FPSCameraController* fpsCam;
	Ptr<Entity> testRamp;
	hagame::math::Surface testSurface;

	Ptr<Entity> sphereA, sphereB;

	void onSceneInit() {
		loadResources();

		addSystem<ImSimMovementSystem>();
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		cameraSystem = addSystem<CameraSystem>();
		addSystem<TestSystem>();
		addSystem<CollisionSystem>();
		mouse = addSystem<MouseSystem>();
		stateSystem = addSystem<StateSystem>();
		addSystem<ProjectileSystem>();
		addSystem<GizmoSystem>();

		projQuad = std::make_shared<Quad>(0.25f, 0.25f);

		player.player = addEntity();
		player.player->addTag("player");
		player.player->transform->move(Vec3::Top(1.5f));
		
		auto movement = player.player->addComponent<PlayerMovement>();

		player.broadCollider = addChild(player.player);

		auto pCollider = player.broadCollider->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 1.5f);

		player.groundCollider = addChild(player.player);
		player.groundCollider->transform->move(Vec3::Top(-1.0f));

		pCollider = player.groundCollider->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 0.2f);

		movement->groundCollider = pCollider;

		player.body = addChild(player.player);
		addCylinder(player.body, 0.25f, 1.0f);
		player.camera = addChild(player.player);
		auto camera = player.camera->addComponent<CameraComponent>();
		auto perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;
		

		player.gun = addChild(player.player);
		player.gun->transform->move(player.player->transform->face() * 2.0f);
		auto meshRenderer = player.gun->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("glock");
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();

		fpsCam = player.player->addComponent<FPSCameraController>();
		fpsCam->camera = perspectiveCam.get();
		fpsCam->ySensitivity = 2.0f;
		fpsCam->xSensitivity = 2.0f;
		fpsCam->inputFn = [this]() {
			FPSCameraControllerParams params;
			params.look = game->input.player(0).rAxis;
			params.zoomed = game->input.player(0).lTrigger;
			return params;
		};

		player.camera->transform->move(Vec3::Top(1.0f));

		debugCameraEntity = addEntity();
		debugCameraEntity->transform->move(Vec3({ 5, 5, 5 }));
		debugCameraEntity->transform->lookAt(player.player->transform->getPosition());
		debugCameraEntity->addComponent<DebugCamera>();
		camera = debugCameraEntity->addComponent<CameraComponent>();
		perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;
		camera->active = false;

		meshRenderer = debugCameraEntity->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("camera");
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();

		addLight(Vec3::Top(10.0f));
		addFloor();

		addCube(Vec3::Zero(), Vec3({ 100, 0.0f, 100 }));
		addCube(Vec3({ -0.5, 0.5, 5 }), Vec3(1.0f));
		addCube(Vec3({ 0.5, 0.5, 5 }), Vec3(1.0f));
		addCube(Vec3({ 0, 1.4f, 5 }), Vec3(0.8f));

		addDisc(Vec3({ 3, 0.5f, -3.0f }), 3.0f);

		sphereA = addCube(Vec3(3.0f), Vec3(0.05f));
		sphereB = addCube(Vec3({ 3.0f, 3.0f, 6.0f }), Vec3(0.05f));

		// addCone(Vec3({ 3, 2.0f, 3 }), 2.0f, 3.0f);

		testRamp = addRamp(Vec3({ -4, 2.5f, 4.0f }), Vec3(5.0f));
		testSurface = testRamp->getComponent<DynamicMeshRenderer>()->mesh->getMesh()->getSurface();
		// std::cout << testMesh.triangles.size() << "\n";

		//addCacodemon(Vec3({ 5, 2, 1 }));
		//addCacodemon(Vec3({ 12, 2, 5 }));

		redDot = addEntity();
		auto dotRenderer = redDot->addComponent<DynamicMeshRenderer>();
		dotRenderer->mesh = std::make_shared<Quad>(0.03f, 0.03f);
		dotRenderer->texture = game->resources->getTexture("red_dot");
		dotRenderer->shader = game->resources->getShaderProgram("texture");
		dotRenderer->color = Color::white();

		auto particles = addEntity();
		auto emitter = particles->addComponent<ParticleEmitterRenderer>();
		emitter->shader = game->resources->getShaderProgram("particle");
		particles->transform->move(Vec3({ 2, 2, 2 }));

		stateSystem->events->subscribe(StateEvents::DebugChange, [this](Ptr<State> state) {
			if (state->debug) {
				cameraSystem->active = false;
				debugCameraEntity->getComponent<CameraComponent>()->active = true;
				player.camera->getComponent<CameraComponent>()->active = false;
			}
			else {
				cameraSystem->active = true;
				debugCameraEntity->getComponent<CameraComponent>()->active = false;
				player.camera->getComponent<CameraComponent>()->active = true;
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

		fpsCam->ySensitivity = game->input.usingGamepad(0) ? stateSystem->state->gamepadSensitivity[0] : stateSystem->state->mouseSensitivity[0];
		fpsCam->xSensitivity = game->input.usingGamepad(0) ? stateSystem->state->gamepadSensitivity[1] : stateSystem->state->mouseSensitivity[1];

		if (game->input.player(0).rTriggerPressed && !stateSystem->state->debug) {
			auto projectile = addEntity();
			auto def = projectile->addComponent<Projectile>();
			def->velocity = player.camera->transform->face() * 30.0f;
			def->gravity = Vec3({ 0, -10.0f, 0.0f });
			projectile->transform->move(player.camera->transform->getPosition() + Vec3::Bottom(0.25f));

			auto emitter = projectile->addComponent<ParticleEmitterRenderer>();
			emitter->shader = game->resources->getShaderProgram("particle");
			emitter->emitter->settings.accel = player.camera->transform->back() * 0.2f;
			emitter->emitter->settings.particlesEmitted = 25;
			emitter->emitter->settings.delay = 0.005f;
			emitter->emitter->settings.minSpeed = 0.290f;
			emitter->emitter->settings.maxSpeed = 0.5f;
			emitter->emitter->settings.minLife = 0.48f;
			emitter->emitter->settings.maxLife = 0.6f;
			emitter->emitter->settings.startColor = Color(0, 255, 170, 128);
			emitter->emitter->settings.endColor = Color(0, 38, 255, 15);
			emitter->emitter->settings.lifeTime = 1.0f;
			emitter->emitter->settings.angle = PI * 2;

			//auto mesh = projectile->addComponent<Sprite3DRenderer>();
			//mesh->sprite = std::make_shared<Sprite3D>();
			//mesh->sprite->quad = projQuad;
			//mesh->sprite->texture = game->resources->getTexture("plasma");
			//mesh->shader = game->resources->getShaderProgram("sprite3d");
		}
	}

	void onSceneBeforeUpdate() {
		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), viewMat, projMat);

		drawCupsuleOutline(player.body->transform->getPosition() + Vec3::Top(0.7f), player.body->transform->getPosition() + Vec3::Bottom(0.7f), 0.6f, Color::blue(), colorShader);

		{
			ImGui::Begin("Input statistics");

			ImGui::DragFloat2("Mouse sensitivity", stateSystem->state->mouseSensitivity.vector, 0.01f, 0.0f, 5.0f);
			ImGui::DragFloat2("Gamepad sensitivity", stateSystem->state->gamepadSensitivity.vector, 0.01f, 0.0f, 5.0f);

			for (int i = 0; i < 4; i++) {
				auto exists = game->input.playerExists(i);
				String text = "Player " + std::to_string(i + 1) + ": ";
				if (exists) {
					text += "connected via " + game->input.playerToString(i);
				}
				else {
					text += "disconnected";
				}

				ImGui::Text(text.c_str());
			}

			ImGui::End();
		}
	}

	void onSceneAfterUpdate() {

		float t;

		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), viewMat, projMat);

		auto planeHit = hagame::math::collisions::checkRayAgainstPlane(mouse->mouseWorldRay, hagame::math::Plane(Vec3::Top(), 1.0f), t);
		if (planeHit.has_value()) {
			drawSphereOutline(planeHit.value().position, 0.3f, Color::red(), colorShader);
		}


		auto tri = hagame::math::Triangle(
			sphereB->transform->getPosition() + (sphereB->transform->left() + sphereB->transform->bottom()).normalized(),
			sphereB->transform->getPosition() + sphereB->transform->top(),
			sphereB->transform->getPosition() + (sphereB->transform->right() + sphereB->transform->bottom()).normalized()
		);

		auto sphere = Sphere(sphereA->transform->getPosition(), 1.0f);
		auto capA = hagame::math::Capsule(hagame::math::LineSegment(sphereA->transform->getPosition() + sphereA->transform->top(), sphereA->transform->getPosition() + sphereA->transform->top() * -1), 0.5f);
		// auto capB = hagame::math::Capsule(hagame::math::LineSegment(sphereB->transform->getPosition() + sphereB->transform->top(), sphereB->transform->getPosition() + Vec3({(float) cos(game->secondsElapsed), 0.0f, (float) sin(game->secondsElapsed)})), 0.5f);
		drawSphereOutline(sphereA->transform->getPosition(), 1.0f, Color::blue(), colorShader);
		// drawSphereOutline(sphereB->transform->getPosition(), 0.5f, Color::green(), colorShader);
		
		drawTriangle(tri, Color::green(), colorShader);

		//auto coll = hagame::math::collisions::checkCapsuleAgainstCapsule(capA, capB);
		//auto coll = hagame::math::collisions::checkCapsuleAgainstTriangle(capA, tri);
		//auto coll = hagame::math::collisions::checkSphereAgainstTriangle(sphere, tri);
		//auto coll = hagame::math::collisions::checkCapsuleAgainstSurface(capA, testSurface, testRamp->transform->getModelMatrix());
		auto coll = hagame::math::collisions::checkSphereAgainstSurface(sphere, testSurface, testRamp->transform->getModelMatrix());
		//drawSphereOutline(sphereA->transform->getPosition(), 1.0f, Color::blue(), colorShader);
		//drawCupsuleOutline(capA.center.a, capA.center.b, capA.radius, Color::blue(), colorShader);
		// drawCupsuleOutline(capB.center.a, capB.center.b, capB.radius, Color::green(), colorShader);

		drawSurface(testSurface, testRamp->transform->getModelMatrix(), Color::red(), colorShader);

		if (coll.has_value()) {

			sphereA->transform->move(coll.value().normal * coll.value().depth);

			drawLine(hagame::math::Line(
				coll.value().position, coll.value().position + coll.value().normal * -coll.value().depth
			), Color::red(), colorShader, 0.005);
			//drawLine(hagame::math::Line(coll.value().position, coll.value().position + coll.value().normal), Color(1.0f, 1.0f, 0.0f), colorShader, 0.005);
			//drawLine(hagame::math::Line(coll.value().position, sphereA->transform->getPosition()), Color(1.0f, 0.0f, 1.0f), colorShader, 0.005);
		}

	}

	Ptr<Entity> addCacodemon(Vec3 pos) {
		auto demon = addEntity();
		demon->transform->setPosition(pos);
		auto mesh = demon->addComponent<Sprite3DRenderer>();
		mesh->sprite = std::make_shared<Sprite3D>();
		mesh->sprite->quad = std::make_shared<Quad>(1.5f, 1.5f);
		mesh->sprite->texture = game->resources->getTexture("cacodemon");
		mesh->shader = game->resources->getShaderProgram("sprite3d");
		return demon;
	}

	Ptr<Entity> addRareRabbit(Vec3 pos) {
		auto demon = addEntity();
		demon->transform->setPosition(pos);
		auto mesh = demon->addComponent<Sprite3DRenderer>();
		mesh->sprite = std::make_shared<Sprite3D>();
		mesh->sprite->quad = std::make_shared<Quad>(1.5f, 1.5f);
		mesh->sprite->texture = game->resources->getTexture("rabbit");
		mesh->shader = game->resources->getShaderProgram("sprite3d");
		return demon;
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

	void addCone(Vec3 pos, float radius, float height) {
		auto entity = addEntity();
		entity->transform->setPosition(pos);
		auto meshRenderer = entity->addComponent<DynamicMeshRenderer>();
		meshRenderer->mesh = std::make_shared<Cone>(radius, height);
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();
		auto collider = entity->addComponent<Collider>();
		collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
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
		auto collider = entity->addComponent<Collider>();
		collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
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
		auto collider = entity->addComponent<Collider>();
		collider->boundingCube = meshRenderer->mesh->getMesh()->getBoundingCube();
		return entity;
	}

	Ptr<Entity> addFloor() {
		auto floor = addEntity();
		auto collider = floor->addComponent<Collider>();
		collider->boundingCube = Cube(Vec3({ -(float)GRID_COLS / 2.0f, 1.0f, -(float)GRID_COLS / 2.0f }), Vec3({ (float)GRID_COLS, 0.0f, (float)GRID_ROWS }));
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
		game->resources->loadTexture("plasma", "Textures/plasma_ball.png");
		game->resources->loadTexture("cacodemon", "Textures/cacodemon.png");
		game->resources->loadTexture("rabbit", "Textures/rarerabbit.jpg");

		Array<String> shaders = {
			"color",
			"texture",
			"text",
			"material",
			"sprite",
			"sprite3d",
			"particle"
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
