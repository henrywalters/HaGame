#include "Game.h"

void hagame::Game::run()
{
	
	onGameStart();

	if (scenes.hasActive()) scenes.active()->activate();

	running = true;

	while (running) {
		long now = hagame::utils::Clock::Now();
		double dt = ((double)now - (double)lastTick) / TICKS_PER_SECOND;
		secondsElapsed += dt;
		fps = TICKS_PER_SECOND / dt;
		lastTick = now;
		window->clear();
		input.pollEvents();
		onGameUpdate(dt);
		if (scenes.hasActive()) {
			scenes.active()->update(dt);
		}
		window->render();
	}

	if (scenes.hasActive()) scenes.active()->deactivate();

	onGameEnd();
}