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

	addCamera();

	for (int i = 0; i < MAP.size(); i++) {
		for (int j = 0; j < MAP[i].size();  j++) {
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

	player->addComponent<PlayerController>();

	addPhysics(player, 1);
	addBoxCollider(player, PLAYER_SIZE);
	
	game->input.keyboardMouse.mouseEvents.subscribe(MouseEvents::Moved, [this](MouseEvent e) {
		setMousePos(e.mousePos);
	});

	game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this](hagame::input::WindowEvent e) {
		setWindowSize(e.data);
	});
}

void Demo::onSceneActivate()
{
	game->input.keyboardMouse.hideCursor();
}

void Demo::onSceneBeforeUpdate()
{
}

void Demo::onSceneUpdate(double dt)
{
	frameTime = dt;
	//auto vel = game->input.keyboardMouse.lAxis * 1 * dt;

	auto playerBody = player->getComponent<RigidBody>();

	
	game->resources->getShaderProgram("text")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], -10, 10));
	drawText(game->resources->getShaderProgram("text"), game->resources->getFont("arial"), std::to_string(dt), Color::white(), Vec3(0, 0));
	drawText(game->resources->getShaderProgram("text"), game->resources->getFont("arial"), player->getPos().toString() + " " + playerBody->vel.toString(), Color::white(), Vec3(0, 30));

	//if (game->input.keyboardMouse.aPressed && collidedDirsY[2]) {
	//	playerBody->applyForce(Vec3::Top(3000.0f));
	//}

	playerBody->applyForce(Vec3::Right(game->input.keyboardMouse.lAxis[0]) * player->getComponent<PlayerController>()->movementForce);

	camera->transform->setPosition(player->transform->getPosition());

}

void Demo::onSceneAfterUpdate()
{
	renderPlayerConfig();
	auto shader = game->resources->getShaderProgram("batch_line");
	shader->use();
	shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(camera->transform.get()));
	lineBuffer.draw();
}

void Demo::onSceneDeactivate()
{
	std::cout << "DEMO DEACTIVATED\n";
	game->input.keyboardMouse.showCursor();
}

void Demo::renderPlayerConfig()
{
	auto platform = player->getComponent<Platformer>();
	auto controller = player->getComponent<PlayerController>();
	ImGui::Begin("Player Controls");
	ImGui::SliderFloat("Movement Force", &controller->movementForce, 0, 100);
	ImGui::SliderFloat("Ground Drag", &platform->groundDrag, 0, 10000);
	ImGui::SliderFloat("Air Drag", &platform->airDrag, 0, 10000);
}
