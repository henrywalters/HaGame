#include "Game.h"

void hagame::Game::run()
{
	onGameStart();

	resources->loadShaderProgram("screen", "Shaders/screen_vert.glsl", "Shaders/screen_frag.glsl");
	window->setRenderShader(resources->getShaderProgram("screen"));

	scenes.forEach([](String name, Ptr<hagame::Scene> scene) {
		scene->initialize();
	});

	if (scenes.hasActive()) scenes.active()->activate();

	running = true;

	while (running) {

		// Handle time tracking
		long now = hagame::utils::Clock::Now();
		double dt = ((double)now - (double)lastTick) / TICKS_PER_SECOND;
		secondsElapsed += dt;
		fps = (double) 1.0 / dt;

		bool physicsUpdate = false;
		double physicsDt = 0;

		if (now - lastPhysicsTick >= PHYSICS_TICK_RATE) {
			physicsUpdate = true;
			physicsDt = ((double)now - (double)lastPhysicsTick) / TICKS_PER_SECOND;
			lastPhysicsTick = now;
		}

		lastTick = now;

		// Update the game state

		// window->clear();
		input.pollEvents();
		onGameBeforeUpdate();
		onGameUpdate(dt);
		
		if (physicsUpdate) {
			onGamePhysicsUpdate(physicsDt);
		}

		if (scenes.hasActive()) {

			scenes.active()->update(dt);

			if (physicsUpdate)
				scenes.active()->onScenePhysicsBeforeUpdate(physicsDt);

			if (sceneChange("physics before update")) continue;

			if (physicsUpdate)
				scenes.active()->ecs.systems.physicsBeforeUpdateAll(physicsDt);

			if (sceneChange("physics systems before update"));

			scenes.active()->ecs.systems.beforeUpdateAll(dt);

			if (sceneChange("before system update")) continue;

			scenes.active()->onSceneBeforeUpdate();

			if (sceneChange("before update")) continue;

			scenes.active()->ecs.systems.updateAll(dt);

			if (sceneChange("scene systems update")) continue;

			scenes.active()->onSceneUpdate(dt);

			if (sceneChange("update")) continue;

			if (physicsUpdate)
				scenes.active()->onScenePhysicsUpdate(physicsDt);

			if (sceneChange("physics update")) continue;

			if (physicsUpdate) {
				scenes.active()->ecs.systems.physicsUpdateAll(physicsDt);
			}

			if (sceneChange("physics systems update")) continue;

			if (physicsUpdate)
				scenes.active()->onScenePhysicsAfterUpdate(physicsDt);

			if (sceneChange("physics after update")) continue;

			if (physicsUpdate)
				scenes.active()->ecs.systems.physicsAfterUpdateAll(physicsDt);

			if (sceneChange("physics systems after update")) continue;

			scenes.active()->ecs.systems.afterUpdateAll(dt);

			if (sceneChange("system after update")) continue;

			scenes.active()->onSceneAfterUpdate();

			if (sceneChange("after update")) continue;
		}
		onGameAfterUpdate();

		scenes.active()->ecs.systems.postTickAll();
		// window->render();
	}

	if (scenes.hasActive()) scenes.active()->deactivate();

	onGameEnd();
}