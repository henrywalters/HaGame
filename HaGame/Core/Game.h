#ifndef GAME
#define GAME
#include "../Utils/Aliases.h"
#include "../Utils/Clock.h"
#include "../Utils/Logger.h"
#include "../Utils/Loggers/ConsoleLogger.h"
#include "../Utils/StateManager.h"
#include "../Input/InputManager.h"
#include "../Graphics/Window.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <SDL.h>

namespace hagame {
	// Base game class that wraps a simple game loop
	class Game {

		utils::StateManager<Scene> scenes;

	public:

		ResourceManager resources;
		input::InputManager input;
		graphics::Window* window;

		const int TICKS_PER_SECOND = 1000000;

		bool running;
		long lastTick;
		double secondsElapsed;
		double fps;
		Ptr<utils::Logger> logger;

		Game(graphics::Window* _window) : 
			scenes(utils::StateManager<Scene>([this](Scene* scene) { scene->activate(); }, [this](Scene* scene) { scene->deactivate(); })), 
			window(_window),
			resources(ResourceManager(""))
		{
			running = false;
			lastTick = utils::Clock::Now();
			secondsElapsed = 0;
			logger = std::make_shared<utils::loggers::ConsoleLogger>();
			Game::input.onQuit([this]() {
				running = false;
			});
		}

		virtual void onGameStart() {};
		virtual void onGameEnd() {};
		virtual void onGameUpdate(double dt) {};

		Scene* addScene(String sceneName) {
			Scene* scene = scenes.add(sceneName);
			scene->game = this;
			return scene;
		}

		static void initializeSDL() {
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
		}

		void run();

		void log(std::string message) {
			logger->log(message);
		}
	};
}

#endif