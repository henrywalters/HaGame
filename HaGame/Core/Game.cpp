#include "Game.h"

void hagame::Game::run()
{

	onGameStart();

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
		fps = (double) TICKS_PER_SECOND / dt;

		bool physicsUpdate = false;
		double physicsDt = 0;

		if (now - lastPhysicsTick >= PHYSICS_TICK_RATE) {
			physicsUpdate = true;
			physicsDt = ((double)now - (double)lastPhysicsTick) / TICKS_PER_SECOND;
			lastPhysicsTick = now;
		}

		lastTick = now;

		// Update the game state

		window->clear();
		input.pollEvents();
		onGameBeforeUpdate();
		onGameUpdate(dt);
		
		if (physicsUpdate) {
			onGamePhysicsUpdate(physicsDt);
		}

		if (scenes.hasActive()) {
			scenes.active()->update(dt);

			if (physicsUpdate) {
				scenes.active()->updatePhysics(physicsDt);
			}
		}
		onGameAfterUpdate();
		window->render();
	}

	if (scenes.hasActive()) scenes.active()->deactivate();

	onGameEnd();
}