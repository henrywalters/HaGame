#include "../../HaGame/HaGame.h"

class FPS : public hagame::Scene {
public:

	const hagame::graphics::Color RED = hagame::graphics::Color(217, 50, 83);
	const hagame::graphics::Color BLUE = hagame::graphics::Color(54, 134, 255);

	hagame::ecs::Entity* player;
	hagame::graphics::PerspectiveCamera camera = hagame::graphics::PerspectiveCamera();

	void setupCamera() {
		camera.aspectRatio = game->window->size[0] / game->window->size[1];
		camera.transform.position = Vec3({ -5.0f, 5.0f, -5.0f });
		camera.transform.rotation = Quat(0.0f, Vec3::Top());
		camera.transform.lookAt(Vec3::Zero());
		camera.transform.lookAt(Vec3({ 5.0f, 5.0f, 5.0f }));
		activeCamera = &camera;
	}

	void initializeResources() {
		game->resources->loadShaderProgram("material", "Shaders/material_vert.glsl", "Shaders/material_frag.glsl");
		game->resources->loadShaderProgram("color", "Shaders/color_vert.glsl", "Shaders/color_frag.glsl");
		game->resources->loadShaderProgram("texture", "Shaders/texture_vert.glsl", "Shaders/texture_frag.glsl");
		game->resources->loadTexture("brick", "Textures/brick.jpg");
		game->resources->loadMesh("glock", "Models/Glock.obj");
		game->resources->loadMesh("cube", "Models/Cube.obj");
	}

	void addSystems() {
		addSystem<hagame::graphics::RenderSystem>();
		addSystem<hagame::physics::PhysicsSystem>();
		addSystem<hagame::physics::MovementSystem>();
		addSystem<hagame::graphics::CameraSystem>();
	}

	void addGameObjects() {
		auto floor = addEntity();
		floor->transform.rotation = Quat(0, Vec3::Top());
		auto fRenderer = floor->addComponent<hagame::graphics::MeshRenderer>();
		fRenderer->mesh = hagame::graphics::Plane<100, 100>(Vec2({ 100, 100 })).generateMesh();
		fRenderer->color = RED;
		fRenderer->shader = game->resources->getShaderProgram("texture");
		fRenderer->texture = game->resources->getTexture("brick");

		auto gun1 = addEntity();
		gun1->transform.rotation = Quat(0, Vec3::Top());
		gun1->transform.position = Vec3({ 10, 2, 10 });
		auto gRenderer = gun1->addComponent<hagame::graphics::MeshRenderer>();
		gRenderer->mesh = game->resources->getMesh("glock");
		gRenderer->color = BLUE;
		gRenderer->shader = game->resources->getShaderProgram("color");
		gRenderer->displayBounds = true;
		auto gRotate = gun1->addComponent<hagame::physics::RotationMovement>();

		player = addEntity();
		player->transform.position = Vec3({ 5.0f, 2.0f, 5.0f });
		player->transform.rotation = Quat(0, Vec3::Top());
		auto rb = player->addComponent<hagame::physics::RigidBody>();
		rb->fixAxis[1] = true;
		auto movementController = player->addComponent<hagame::physics::SimpleMovement>();
		movementController->inputFn = [this]() {
			hagame::physics::SimpleMovementParameters params;
			params.movement = game->input.keyboardMouse.lAxis.normalized();
			params.running = game->input.keyboardMouse.isKeyDown(SDLK_LSHIFT);
			return params;
		};

		auto cam = player->addComponent<hagame::graphics::FPSCameraController>();
		cam->camera = &camera;
		cam->inputFn = [this]() {
			hagame::graphics::FPSCameraControllerParams params;
			params.look = game->input.keyboardMouse.rAxis;
			params.zoomed = game->input.keyboardMouse.mouse.right;
			return params;
		};


		auto pRenderer = player->addComponent<hagame::graphics::MeshRenderer>();
		pRenderer->mesh = game->resources->getMesh("cube");
		pRenderer->color = BLUE;
		pRenderer->shader = game->resources->getShaderProgram("color");
	}

	void onSceneActivate() {
		std::cout << "ACtivating FPS";
		initializeResources();
		setupCamera();
		addSystems();
		addGameObjects();
	}

	void onSceneUpdate(double dt) {
		activeCamera->transform.position = player->transform.position + Vec3({ 0, 2.0, 0 });
		// activeCamera->transform.lookAt(player->transform.position + player->transform.face());
	}
};