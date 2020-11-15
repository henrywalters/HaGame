#include "Game.h"

void hagame::Game::run()
{
	onGameStart();
	running = true;

	while (running) {
		long now = hagame::utils::Clock::Now();
		double dt = now - lastTick;
		lastTick = now;
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		onGameUpdate(dt / 1000000.0);
		
	}

	onGameEnd();
}
