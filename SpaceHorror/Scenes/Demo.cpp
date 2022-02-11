#include "Demo.h"

using namespace hagame::input::devices;
using namespace hagame::graphics;
using namespace hagame::physics;

void Demo::onSceneInit()
{
	std::cout << "DEMO INITIALIZED\n";

	game->window->turnOnVSync();

	game->window->clearColor = Color("#003153");

	addSystem<RenderSystem>();
	addSystem<PhysicsSystem>();
	addSystem<PartitionSystem>();
	addSystem<CollisionSystem>();

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
				player = addSprite("cacodemon", Vec2(j, i).prod(BLOCK_SIZE), PLAYER_SIZE);
				break;
			}
		}
	}

	//auto crosshair = addSprite("crosshair", Vec2::Zero(), Vec2(0.1f));

	addPhysics(player, 1);
	addBoxCollider(player, PLAYER_SIZE);

	game->input.keyboardMouse.mouseEvents.subscribe(MouseEvents::Moved, [this](MouseEvent e) {
		setMousePos(e.mousePos);
		//crosshair->transform->setPosition(mousePos);
		//std::cout << e.mousePos << "\n";
	});

	game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this](hagame::input::WindowEvent e) {
		setWindowSize(e.data);
	});
}

void Demo::onSceneActivate()
{
	std::cout << "DEMO ACTIVATED\n";
	game->input.keyboardMouse.hideCursor();
}

void Demo::onSceneBeforeUpdate()
{
	
	//Vec2 vel = player->getComponent<RigidBody>()->vel * frameTime;

	
}

void Demo::onSceneUpdate(double dt)
{
	frameTime = dt;
	//auto vel = game->input.keyboardMouse.lAxis * 1 * dt;

	auto playerBody = player->getComponent<RigidBody>();

	
	game->resources->getShaderProgram("text")->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], -10, 10));
	drawText(game->resources->getShaderProgram("text"), game->resources->getFont("arial"), std::to_string(dt), Color::white(), Vec3(0, 0));
	drawText(game->resources->getShaderProgram("text"), game->resources->getFont("arial"), player->getPos().toString() + " " + playerBody->vel.toString(), Color::white(), Vec3(0, 30));

	//drawRect(Rect(player->transform->getPosition(), PLAYER_SIZE), Color::blue());
	auto cube = player->getComponent<Collider>()->boundingCube.value(); 
	//cube.pos += player->transform->getPosition();

	if (game->input.keyboardMouse.a) {
		playerBody->applyForce(Vec3::Top(15.0f));
	}

	playerBody->applyForce(Vec3::Right(game->input.keyboardMouse.lAxis[0]) * 5.0f);

	drawCubeOutline(cube, Color::green());

	drawLine(hagame::math::Line(player->getPos(), mousePos), Color::blue());

	ecs.entities.forEach([this](Ptr<hagame::ecs::Entity> entity) {
		auto collider = entity->getComponent<Collider>();
		if (collider != NULL) {
			// std::cout << entity->name << "\n";
			float t;
			auto cube = collider->boundingCube.value();
			cube.pos += entity->getPos();
			auto ray = hagame::math::Ray(player->getPos(), mousePos - player->getPos());
			auto hit = hagame::math::collisions::checkRayAgainstRect(ray, Rect(cube.pos, cube.size), t);

			if (hit.has_value() && t <= 1.0f) {
				drawCircle2D(ray.getPointOnLine(t), 0.1, Color::red());
				drawLine(hagame::math::Line(hit.value().position, hit.value().position + hit.value().normal * 0.5f), Color::green());
			}
		}
	}, { player->uuid }, { "player" });

		Vec3 vel = player->getComponent<RigidBody>()->vel; //game->input.keyboardMouse.lAxis * frameTime;
	//vel += game->input.keyboardMouse.lAxis;
	vel *= frameTime;
	bool grounded = false;
	resolveCollisions(player, vel, grounded);

	if (grounded && player->getComponent<RigidBody>()->vel[1] < 0.0f) {
		player->getComponent<RigidBody>()->vel[1] = 0.0f;
	}

	player->transform->move(vel);

	camera->transform->setPosition(player->transform->getPosition());

	//player->getComponent<RigidBody>()->clearForces();

	/*
	if (game->collisions.checkCollisions(player, player->getComponent<Collider>(), vel, dt, t)) {
		std::cout << t << "\n";
		player->transform->move(vel * dt * t * 0.5f);
	}
	else {
		
	}
	*/
}

void Demo::onSceneAfterUpdate()
{


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
