#ifndef GAME
#define GAME
#include "../Utils/Aliases.h"
#include "../Utils/Clock.h"
#include "../Utils/Logger.h"
#include "../Utils/Loggers/ConsoleLogger.h"
#include "../Utils/StateManager.h"
#include "Scene.h"
#include <SDL.h>

namespace hagame {
	// Base game class that wraps a simple game loop
	class Game {
	public:

		utils::StateManager<Scene> scenes;

		const int TICKS_PER_SECOND = 1000000;

		bool running;
		long lastTick;
		double secondsElapsed;
		double fps;
		Ptr<utils::Logger> logger;


		Game() {
			running = false;
			lastTick = utils::Clock::Now();
			secondsElapsed = 0;
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