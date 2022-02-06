#ifndef IMSIM_LEVEL_1_HPP
#define IMSIM_LEVEL_1_HPP

#include "../../HaGame/HaGame.h"
#include "../Systems/MouseSystem.hpp"
#include "../Systems/StateSystem.hpp"
#include "../Systems/Movement.hpp"
#include "../Systems/ProjectileSystem.hpp"
#include "../Systems/GizmoSystem.hpp"
#include "../Systems/WeaponSystem.hpp"
#include "../Components/PlayerMovement.hpp"
#include "../Common/RuntimeLevel.hpp"
#include "../Components/Player.hpp"

using namespace hagame::ecs;
using namespace hagame::graphics;

class Level1 : public RuntimeLevel {
public:
	Player player;
	Ptr<Entity> gun;
	Ptr<Entity> light;
	Ptr<Entity> debugCameraEntity;
	Ptr<Entity> floor;
	Ptr<MouseSystem> mouse;
	Ptr<Quad> projQuad;
	Ptr<WeaponSystem> weaponSystem;
	Ptr<CameraSystem> cameraSystem;
	Ptr<StateSystem> stateSystem;
	Ptr<OrthographicCamera> orthCamera;
	FPSCameraController* fpsCam;
	hagame::math::Surface floorSurface;

	bool isDrawing = false;
	bool hasLastPos = false;
	Vec3 lastPos;
	Ptr<LineBuffer> currentLine;
	Array<Ptr<LineBuffer>> lines;
	Ptr<DynamicMesh> cubeMesh;
	Ptr<MeshBuffer> cubeBuffer;

	Array<Ptr<Entity>> cubes;

	void onSceneInit() {

		addSystem<ImSimMovementSystem>();
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		cameraSystem = addSystem<CameraSystem>();
		addSystem<TestSystem>();
		addSystem<CollisionSystem>();
		mouse = addSystem<MouseSystem>();
		stateSystem = addSystem<StateSystem>();
		weaponSystem = addSystem<WeaponSystem>();
		addSystem<ProjectileSystem>();
		addSystem<GizmoSystem>();
		addSystem<hagame::physics::PartitionSystem>();

		projQuad = std::make_shared<Quad>(10.0f, 10.0f);

		player.player = addEntity();
		player.player->addTag("player");
		player.player->transform->move(Vec3::Top(1.5f));
		
		auto movement = player.player->addComponent<PlayerMovement>();

		player.broadCollider = addChild(player.player);
		player.broadCollider->addTag("player");

		auto pCollider = player.broadCollider->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 1.5f);
		//pCollider->boundingVolume = BoundingSphere(hagame::math::NSphere<3, float>(Vec3::Zero(), 1.5f));

		player.groundCollider = addChild(player.player);
		player.groundCollider->addTag("player");
		player.groundCollider->transform->move(Vec3::Top(-1.0f));

		pCollider = player.groundCollider->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 0.2f);
		//pCollider->boundingVolume = BoundingSphere(hagame::math::NSphere<3, float>(Vec3::Zero(), 0.2f));

		movement->groundCollider = pCollider;

		player.body = addChild(player.player);
		player.body->addTag("player");
		addCylinder(player.body, 0.25f, 1.0f);
		player.camera = addChild(player.player);
		auto camera = player.camera->addComponent<CameraComponent>();
		auto perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;

		/*auto pVolume = player.body->addComponent<hagame::physics::BoundingVolume>(
			hagame::math::Capsule(
				hagame::math::LineSegment(Vec3::Bottom(1.0f), Vec3::Top(1.0f)), 0.5f)
			);
			*/

		auto pVolume = player.body->addComponent<hagame::physics::BoundingVolume>(Cube(Vec3(-0.5f, -1.0f, -0.5f), Vec3(1, 2, 1)));

		pVolume->dynamic = true;

		orthCamera = std::make_shared<OrthographicCamera>(game->window->size);

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
		player.camera->addTag("player");

		debugCameraEntity = addEntity();
		debugCameraEntity->transform->move(Vec3({ 5, 5, 5 }));
		debugCameraEntity->transform->lookAt(player.player->transform->getPosition());
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

		//floor = addFloor<10, 10>(Vec3::Zero(), Vec2(100.0f));
		//floor->name = "Floor";
		//floorSurface = floor->getComponent<DynamicMeshRenderer>()->mesh->getMesh()->getSurface();
		//auto floorVolume = floor->addComponent<BoundingVolume>(floorSurface);

		//addMesh(Vec3::Zero(), Vec3(1.0f), "church", "proto");

		//addCube(Vec3::Zero(), Vec3(50.0f, 0.0f, 50.0f));

		//addCube(Vec3(3, 0.5, 3), Vec3(1.0f));
		//addCube(Vec3(3, 1.5, 5), Vec3(1.0f));

		//addCube(Vec3(0, 4, -5), Vec3(10.0f, 4.0f, 0.2f));

		addSprite2D(Vec2(50.0f, 50.0f), Vec2(100), "crosshairs");

		addCacodemon(Vec3(7.0f));

		//addRamp(Vec3(-3, 1.0, 3), Vec3(1.0f, 1.0f, 2.0f));

		int cubeGridSize = 1;

		for (int i = 0; i < cubeGridSize; i++) {
			for (int j = 0; j < cubeGridSize; j++) {
				addCube(Vec3(i * 2.0f, 0.75, 2.0f * j), Vec3(1.5f));
			}
		}

		cubeBuffer = std::make_shared<MeshBuffer>();
		cubeMesh = std::make_shared<RectPrism>(Vec3(1.5f));
		cubeBuffer->initializeBuffers(cubeMesh->getMesh());

		int size = 2;
		float padding = 2;
		float offset = 5;
		float radius = 10;

		for (int i = 0; i < 360; i++) {
			float x = sin(i * DEG_TO_RAD) * radius + 15;
			float y = cos(i * DEG_TO_RAD) * radius + 15;
			cubeBuffer->insert(Mat4::Translation(Vec3(x, y, 0.0f)), Mat4::Identity(), Mat4::Identity());
		}

		addLight(Vec3(0, 10, 0));



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
	}

	void onSceneBeforeUpdate() {

		OrthographicCamera camera = OrthographicCamera(game->window->size);

		if (game->input.keyboardMouse.keyboard.numbersPressed[1]) {
			hagame::graphics::BATCH_LINE_RENDERING = !hagame::graphics::BATCH_LINE_RENDERING;
		}

		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), viewMat, projMat);

		hagame::math::collisions::DEBUG_SHADER = colorShader;
		hagame::graphics::DEBUG_SHADER = colorShader;

		auto vol = player.body->getComponent<BoundingVolume>();
		auto volCube = vol->getBoundingBox().getCube();
		// volCube.pos += player.body->transform->getPosition();
		drawCubeOutline(volCube, Color::blue());

		// drawSurface(floorSurface, floor->transform->getModelMatrix(), Color::green());

		auto lineShader = game->resources->getShaderProgram("batch_line");
		lineShader->use();
		lineShader->setMVP(Mat4::Identity(), viewMat, projMat);
		hagame::graphics::lineBuffer.draw();

		// drawCubeOutline(getSystem<hagame::physics::PartitionSystem>()->getStaticEntities()->getPartition(volCube.pos).getCube(), Color::blue());

		auto neighbors = getSystem<hagame::physics::PartitionSystem>()->getStaticEntities()->getNeighbors(vol, player.body->transform->getPosition());

		if (DEBUG_GRAPHICS) {
			std::cout << "Checking " << neighbors.size() << " neighbors\n";
			player.body->getComponent<BoundingVolume>()->getCube().pos = player.body->transform->getPosition();
			for (auto neighbor : neighbors) {
				//std::cout << neighbor->name << "\n";
				drawCubeOutline(Cube(neighbor->transform->getPosition() - Vec3(0.75f), Vec3(1.5f)), Color::red());

				if (!neighbor->hasTag("player")) {
					auto hit = game->collisions.checkCollision(
						player.body->getComponent<BoundingVolume>(), 
						player.body->transform->getPosition(), 
						neighbor->getComponent<BoundingVolume>(),
						neighbor->transform->getPosition()
					);
					if (hit.has_value()) {
						std::cout << neighbor->name << "\n";
					}
				}
			}
		}

		auto spriteShader = game->resources->getShaderProgram("sprite2d");
		spriteShader->use();
		spriteShader->setMVP(Mat4::Identity(), Mat4::Identity(), projMat);
		projQuad->getMesh()->draw();

		hagame::math::Ray mouseWorldRay;
		auto mousePos = game->input.keyboardMouse.mouse.position;
		auto mouseWorldPos = MouseToWorld(viewMat, projMat, mousePos, game->window->size);
		mouseWorldRay.origin = activeCameraEntity->transform->getPosition();
		mouseWorldRay.direction = (mouseWorldPos - activeCameraEntity->transform->getPosition()).normalized() * CAMERA_RAY_DISTANCE;

		float t;
		//auto rayHit = game->collisions.raycast(player.body, mouseWorldRay, t, {"player"});
		auto rayHit = game->collisions.raycast(mouseWorldRay, ecs.entities, t, { "player" });

		if (game->input.keyboardMouse.mouse.leftPressed) {
			currentLine = std::make_shared<LineBuffer>();
			currentLine->initializeBuffers();
			lines.push_back(currentLine);
			isDrawing = true;
		}
		

		if (game->input.keyboardMouse.mouse.left) {
			if (rayHit.has_value() && isDrawing) {
				if (!hasLastPos) {
					lastPos = rayHit.value().point;
					hasLastPos = true;
				}
				else {
					currentLine->insert(hagame::math::Line(lastPos, rayHit.value().point), Color::blue(), 0.01f);
					lastPos = rayHit.value().point;
				}
			}
		}
		else {
			isDrawing = false;
			hasLastPos = false;
		}

		

		lineShader->use();
		for (auto line : lines) {
			line->draw(false);
		}

		auto textureShader = game->resources->getShaderProgram("batch_texture");
		textureShader->use();
		textureShader->setMVP(Mat4::Rotation(Quat(game->secondsElapsed, Vec3::Right())), viewMat, projMat);
		textureShader->setVec3("viewPos", activeCameraEntity->transform->getPosition());
		// textureShader->setMat4("normal", modelInverse);
		textureShader->setVec4("color", Color::white());
		textureShader->setMaterial("material", Material::blackRubber());

		auto texture = game->resources->getTexture("crate");

		glActiveTexture(GL_TEXTURE0 + 0);
		texture->bind();
		glActiveTexture(GL_TEXTURE0 + 1);
		texture->bind();
		glActiveTexture(GL_TEXTURE0 + 2);
		texture->bind();


		cubeBuffer->draw(false);

	}

	void onSceneAfterUpdate() {

	}
};

#endif
