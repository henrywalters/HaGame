#ifndef GAME
#define GAME
#include "../Utils/Aliases.h"
#include "../Utils/Clock.h"
#include "../Utils/Logger.h"
#include "../Utils/Loggers/ConsoleLogger.h"
#include <SDL.h>

namespace hagame {
	// Base game class that wraps a simple game loop
	class Game {
	public:

		bool running;
		long lastTick;
		Ptr<utils::Logger> logger;

		Game() {
			running = false;
			lastTick = utils::Clock::Now();
			logger = std::make_shared<utils::loggers::ConsoleLogger>();
		}

		virtual void onGameStart() {};
		virtual void onGameEnd() {};
		virtual void onGameUpdate(double dt) {};

		void run();
		
		void log(std::string message) {
			logger->log(message);
		}
	};
}

#endif