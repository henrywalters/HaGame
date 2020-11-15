#ifndef GAME
#define GAME
#include "../Utils/Aliases.h"
#include "../Utils/Clock.h"
#include <SDL.h>

namespace hagame {
	// Base game class that wraps a simple game loop
	class Game {
	public:
		bool running;
		long lastTick;

		Game() {
			running = false;
			lastTick = hagame::utils::Clock::Now();
		}

		virtual void onGameStart() {};
		virtual void onGameEnd() {};
		virtual void onGameUpdate(double dt) {};

		void run();
	};
}

#endif