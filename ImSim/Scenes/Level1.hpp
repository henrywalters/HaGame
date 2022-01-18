#ifndef IMSIM_LEVEL_1_HPP
#define IMSIM_LEVEL_1_HPP

#include "../../HaGame/HaGame.h"
#include "../Systems/MouseSystem.hpp"
#include "../Systems/StateSystem.hpp"
#include "../Systems/Movement.hpp"
#include "../Systems/ProjectileSystem.hpp"
#include "../Systems/GizmoSystem.hpp"
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
	Ptr<CameraSystem> cameraSystem;
	Ptr<StateSystem> stateSystem;
	FPSCameraController* fpsCam;
	hagame::math::Surface floorSurface;

	void onSceneInit() {

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
		addSystem<hagame::physics::PartitionSystem>();

		projQuad = std::make_shared<Quad>(0.25f, 0.25f);

		player.player = addEntity();
		player.player->addTag("player");
		player.player->transform->move(Vec3::Top(1.5f));
		
		auto movement = player.player->addComponent<PlayerMovement>();

		player.broadCollider = addChild(player.player);

		auto pCollider = player.broadCollider->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 1.5f);
		//pCollider->boundingVolume = BoundingSphere(hagame::math::NSphere<3, float>(Vec3::Zero(), 1.5f));

		player.groundCollider = addChild(player.player);
		player.groundCollider->transform->move(Vec3::Top(-1.0f));

		pCollider = player.groundCollider->addComponent<Collider>();
		pCollider->type = ColliderType::SphereCollider;
		pCollider->boundingSphere = hagame::math::NSphere<3, float>(Vec3::Zero(), 0.2f);
		//pCollider->boundingVolume = BoundingSphere(hagame::math::NSphere<3, float>(Vec3::Zero(), 0.2f));

		movement->groundCollider = pCollider;

		player.body = addChild(player.player);
		addCylinder(player.body, 0.25f, 1.0f);
		player.camera = addChild(player.player);
		auto camera = player.camera->addComponent<CameraComponent>();
		auto perspectiveCam = std::make_shared<PerspectiveCamera>();
		perspectiveCam->setAspectRatio(game->window);
		camera->camera = perspectiveCam;

		auto pVolume = player.body->addComponent<hagame::physics::BoundingVolume>(
			hagame::math::Capsule(
				hagame::math::LineSegment(Vec3::Bottom(-1.0f), Vec3::Top(1.0f)), 0.5f)
			);

		pVolume->dynamic = true;

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

		auto meshRenderer = debugCameraEntity->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("camera");
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();

		floor = addFloor<10, 10>(Vec3::Zero(), Vec2(100.0f));
		floor->name = "Floor";
		floorSurface = floor->getComponent<DynamicMeshRenderer>()->mesh->getMesh()->getSurface();
		auto floorVolume = floor->addComponent<BoundingVolume>(floorSurface);

		// addCube(Vec3::Zero(), Vec3(50.0f, 0.0f, 50.0f));

		//addCube(Vec3(3, 0.5, 3), Vec3(1.0f));
		//addCube(Vec3(3, 1.5, 5), Vec3(1.0f));

		//addRamp(Vec3(-3, 1.0, 3), Vec3(1.0f, 1.0f, 2.0f));

		//for (int i = 0; i < 10; i++) {
		//	for (int j = 0; j < 10; j++) {
		//		addCube(Vec3(5 + i * 4, 0.5, 5 + j * 4), Vec3(1.5f));
		//	}
		//}

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
		
		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), viewMat, projMat);

		hagame::math::collisions::DEBUG_SHADER = colorShader;
		hagame::graphics::DEBUG_SHADER = colorShader;

		auto vol = player.body->getComponent<BoundingVolume>();
		auto volCube = vol->getBoundingBox().getCube();
		volCube.pos += player.body->transform->getPosition();
		drawCubeOutline(volCube, Color::blue());

		drawSurface(floorSurface, floor->transform->getModelMatrix(), Color::green());

		// drawCubeOutline(getSystem<hagame::physics::PartitionSystem>()->getStaticEntities()->getPartition(volCube.pos).getCube(), Color::blue());

		auto neighbors = getSystem<hagame::physics::PartitionSystem>()->getStaticEntities()->getNeighbors(vol, player.body->transform->getPosition());

		if (DEBUG_GRAPHICS) {
			for (auto neighbor : neighbors) {
				std::cout << neighbor->name << "\n";
				drawCubeOutline(Cube(neighbor->transform->getPosition() - Vec3(0.75f), Vec3(1.5f)), Color::red());
			}
		}

	}

	void onSceneAfterUpdate() {

	}
};

#endif
