#include "Demo.h"

using namespace hagame::input::devices;
using namespace hagame::graphics;
using namespace hagame::physics;

void Demo::onSceneInit()
{
	std::cout << "DEMO INITIALIZED\n";

	// game->window->turnOnVSync();

	game->window->clearColor = Color("#003153");



	addCamera();
	uiOrth = std::make_shared<OrthographicCamera>(game->window->size);
	uiOrth->centered = false;

	addHUD();

	Vec3 start, end;
	RawPtr<Entity> mover;

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
				player = addEXR("test_exr", Vec2(j, i).prod(BLOCK_SIZE), PLAYER_SIZE);
				break;
			case 'W':
				walkers.push_back(addQuad(Vec2(j, i).prod(BLOCK_SIZE) - WALKER_SIZE * 0.5f, WALKER_SIZE, PRIMARY));
				break;
			case 'S':
				start = Vec2(j, i).prod(BLOCK_SIZE);
				break;
			case 'E':
				end = Vec2(j, i).prod(BLOCK_SIZE);
				break;
			case 'M':
				mover = addSprite("prototype", Vec2(j, i).prod(BLOCK_SIZE), BLOCK_SIZE);
				break;
			case 'L':
				light = addLight(Vec2(j, i).prod(BLOCK_SIZE), 10.0f);
				break;
			case 'B':
				addBoxCollider(
					addSprite("explosive_barrel", Vec2(j, i).prod(BLOCK_SIZE), BLOCK_SIZE),
					BLOCK_SIZE,
					false
				);
				break;
			}
		}
	}
	
	std::cout << start << " -> " << end << "\n";

	//auto mPlatform = mover->addComponent<MovingPlatform>();
	//mPlatform->start = start;
	//mPlatform->end = end;
	//addBoxCollider(mover, BLOCK_SIZE, true);

	player->addTag("player");
	player->name = "player";
	player->addComponent<PlayerController>();
	auto blastEmitter = player->addComponent<ParticleEmitterRenderer>();
	blastEmitter->shader = game->resources->getShaderProgram("particle");
	auto weaponController = player->addComponent<WeaponController>();
	weaponController->playerControlled = true;

	addPhysics(player, 1);
	addBoxCollider(player, PLAYER_SIZE);

	for (auto walker : walkers) {
		// walker->getComponent<QuadRenderer>()->quad->setOrigin(Vec2::Zero());
		walker->addComponent<Walker>();
		auto health = walker->addComponent<Health>();
		auto healthRenderer = walker->addComponent<HealthBarRenderer>(Vec2(0.3, 0.05), health->maxHealth);
		healthRenderer->getQuad()->setOffset(Vec2(-0.15, WALKER_SIZE[1]));
		healthRenderer->shader = game->resources->getShaderProgram("color");
		walker->name = "Walker";
		addPhysics(walker, 0.5f);
		addBoxCollider(walker, WALKER_SIZE);
	}

	game->input.keyboardMouse.mouseEvents.subscribe(MouseEvents::Moved, [this](MouseEvent e) {
		//setMousePos(e.mousePos);
	});

	game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this](hagame::input::WindowEvent e) {
		setWindowSize(e.data);
		game->window->setViewport(Rect(Vec2::Zero(), e.data));
	});

}

void Demo::onSceneActivate()
{
	game->window->ambientColor = Color::black();
	game->window->clearColor = Color::black();
	player->getComponent<WeaponController>()->setWeapon(getSystem<WeaponSystem>()->getWeaponByName("AssaultRifle"), 3, 12);
}

void Demo::onSceneBeforeUpdate()
{
	setMousePos(game->input.keyboardMouse.mouse.position);
	getSystem<WeaponSystem>()->setMousePos(mousePos);
}

void Demo::onSceneUpdate(double dt)
{

	if (game->input.keyboardMouse.keyboard.numbersPressed[1]) {
		std::cout << "RECOMPILING Shaders\n";
		game->resources->recompileShaderProgram("light2d", "Shaders/light2d_vert.glsl", "Shaders/light2d_frag.glsl");
		game->resources->recompileShaderProgram("screen", "Shaders/screen_vert.glsl", "Shaders/screen_frag.glsl");
	}

	frameTime = dt;
	auto playerBody = player->getComponent<RigidBody>();

	ImGui::Begin("Light Settings");
	ImGui::ColorEdit4("Color", lColor.vector);
	ImGui::SliderFloat("Constant", &lConst, 0.0, 10.0);
	ImGui::SliderFloat("Linear", &lLin, 0.0, 10.0);
	ImGui::SliderFloat("Quadratic", &lQuad, 0.0, 10.0);

	light->setPos(mousePos);

	auto discLight = light->getComponent<DiscLightRenderer>();

	discLight->color = lColor;
	discLight->constant = lConst;
	discLight->linear = lLin;
	discLight->quadratic = lQuad;

	if (game->input.keyboardMouse.mouse.leftPressed && !guiActive()) {
		discLight = addLight(mousePos, 5.0f)->getComponent<DiscLightRenderer>();
		discLight->color = lColor;
		discLight->constant = lConst;
		discLight->linear = lLin;
		discLight->quadratic = lQuad;
	}

	ImGui::Begin("Entity Tree");
	ecs.entities.forEach([this](auto entity) {
		if (entity->parent == nullptr)
			entityTree.render(game, entity);
	});
	ImGui::End();

	renderPlayerConfig();
}

void Demo::onSceneAfterUpdate()
{

	auto textShader = game->resources->getShaderProgram("text");
	textShader->use();
	textShader->setMVP(Mat4::Identity(), Mat4::Identity(), uiOrth->getProjMatrix(Vec3::Zero()));

	camera->transform->setPosition(player->transform->getPosition());

	auto shader = game->resources->getShaderProgram("batch_line");
	shader->use();
	shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(camera->transform.get()));
	lineBuffer.draw();

	if (game->input.keyboardMouse.start) {
		game->setScene("home");
	}
}

void Demo::onSceneDeactivate()
{
	std::cout << "DEMO DEACTIVATED\n";
	//ecs.entities.clear();
	// lineBuffer.clear();
	//game->input.keyboardMouse.showCursor();
	
}

void Demo::renderPlayerConfig()
{
	auto platform = player->getComponent<Platformer>();
	auto controller = player->getComponent<PlayerController>();
	auto rigidbody = player->getComponent<RigidBody>();
	
	ImGui::Begin("Player Settings");
	platform->updateImGui();
	controller->updateImGui();
	ImGui::Text(rigidbody->vel.toString().c_str()); 
	ImGui::DragFloat3("Gravity", rigidbody->forceDueToGravity.vector, 0.1f, -100.0f, 100.0f);
	ImGui::End();
}

bool Demo::guiActive()
{
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
}
