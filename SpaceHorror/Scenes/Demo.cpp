#include "Demo.h"

using namespace hagame::input::devices;
using namespace hagame::graphics;
using namespace hagame::physics;

void Demo::onSceneInit()
{
	std::cout << "DEMO INITIALIZED\n";

	// game->window->turnOnVSync();

	game->window->clearColor = Color("#003153");

	addSystem<PhysicsSystem>();
	addSystem<RenderSystem>();
	addSystem<PartitionSystem>();
	addSystem<CollisionSystem>();
	addSystem<PlatformerSystem>();


}

void Demo::onSceneActivate()
{
	addCamera();

	for (int i = 0; i < MAP.size(); i++) {
		for (int j = 0; j < MAP[i].size(); j++) {
			switch (MAP[i][j]) {
			case '#':
				addBoxCollider(
					addSprite("prototype", Vec2(j, i).prod(BLOCK_SIZE), BLOCK_SIZE),
					BLOCK_SIZE,
					false
				);
				break;
			case 'P':
				player = addSprite("henry", Vec2(j, i).prod(BLOCK_SIZE), PLAYER_SIZE);
				break;
			}
		}
	}

	auto bulletConfig = hagame::utils::File(CONFIG_DIR, "Bullets.conf");
	auto weaponConfig = hagame::utils::File(CONFIG_DIR, "Weapons.conf");

	bullets = parseBulletConfig(bulletConfig);
	weapons = parseWeaponConfig(weaponConfig, bullets);

	player->addComponent<PlayerController>();

	addPhysics(player, 1);
	addBoxCollider(player, PLAYER_SIZE);

	game->input.keyboardMouse.mouseEvents.subscribe(MouseEvents::Moved, [this](MouseEvent e) {
		//setMousePos(e.mousePos);
	});

	game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this](hagame::input::WindowEvent e) {
		setWindowSize(e.data);
		game->window->setViewport(Rect(Vec2::Zero(), e.data));
	});
}

void Demo::onSceneBeforeUpdate()
{
	setMousePos(game->input.keyboardMouse.mouse.position);

	auto playerController = player->getComponent<PlayerController>();

	if (!game->input.usingGamepad(0)) {
		drawCrosshairs(mousePos, 0.1, 0.3, Color::green());
		playerController->lookingAt = mousePos;
	}
	else {
		auto looking = game->input.player(0).rAxis;
		if (abs(looking[1]) > 0.1f) {
			lookAngle += looking[1] * frameTime;
		}

		if (looking[0] > 0.9f) {
			lookAngle = 0;
		}

		if (looking[0] < -0.9f) {
			lookAngle = PI;
		}

		playerController->lookingAt = player->getPos() + Quat(lookAngle, Vec3::Face()).rotatePoint(Vec3::Right());
	}
}

void Demo::onSceneUpdate(double dt)
{

	frameTime = dt;
	auto playerBody = player->getComponent<RigidBody>();

	if (game->input.keyboardMouse.start) {
		game->scenes.activate("home");
	}
}

void Demo::onSceneAfterUpdate()
{
	camera->transform->setPosition(player->transform->getPosition());

	auto shader = game->resources->getShaderProgram("batch_line");
	shader->use();
	shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(camera->transform.get()));
	lineBuffer.draw();
}

void Demo::onSceneDeactivate()
{
	std::cout << "DEMO DEACTIVATED\n";
	game->input.keyboardMouse.showCursor();
	ecs.entities.clear();
}

void Demo::renderPlayerConfig()
{
	auto platform = player->getComponent<Platformer>();
	auto controller = player->getComponent<PlayerController>();
	auto rigidbody = player->getComponent<RigidBody>();

	ImGui::Begin("Player Controls");
	ImGui::SliderFloat("Movement Force", &controller->movementForce, 0, 10000);
	ImGui::SliderFloat("Jump Force", &controller->jumpForce, 0, 10000);
	ImGui::SliderFloat("Ground Drag", &platform->groundDrag, 0, 10000);
	ImGui::SliderFloat("Air Drag", &platform->airDrag, 0, 10000);
	ImGui::SliderFloat("Gravity", &rigidbody->forceDueToGravity, -30, 30);
}
