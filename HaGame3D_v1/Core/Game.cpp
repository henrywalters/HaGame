#include "Game.h"

void hagame::Game::run()
{

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);

	onGameStart();
	running = true;

	while (running) {
		long now = hagame::utils::Clock::Now();
		double dt = (now - lastTick) / 1000000.0;
		lastTick = now;
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		onGameUpdate(dt);
		
	}

	onGameEnd();
}
