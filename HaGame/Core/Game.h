#ifndef GAME
#define GAME
#include "../Utils/Aliases.h"
#include "../Utils/Clock.h"
#include "../Utils/Logger.h"
#include "../Utils/Loggers/ConsoleLogger.h"
#include "../Utils/StateManager.h"
#include "../Utils/Random.h"
#include "../Physics/Collisions.h"
#include "../Input/InputManager.h"
#include "../Graphics/Window.h"
#include "../Audio/Audio.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <SDL.h>

namespace hagame {
	// Base game class that wraps a simple game loop
	class Game {
	public:

		utils::StateManager<Scene> scenes;
		Ptr<ResourceManager> resources;
		input::InputManager input;
		graphics::Window* window;
		Ptr<audio::Audio> audio;
		hagame::utils::Random random;
		physics::Collisions collisions;

		const long TICKS_PER_SECOND = 1000000;
		const long PHYSICS_TICK_RATE = 16000;

		bool running;
		long lastTick;
		long lastPhysicsTick;
		double secondsElapsed;
		double fps;
		Ptr<utils::Logger> logger;

		Game(graphics::Window* _window) : 
			scenes(utils::StateManager<Scene>([this](Scene* scene) { scene->activate(); }, [this](Scene* scene) { scene->deactivate(); })), 
			window(_window),
			resources(std::make_shared<ResourceManager>("")),
			audio(std::make_shared<audio::Audio>())
		{
			running = false;
			lastTick = utils::Clock::Now();
			lastPhysicsTick = utils::Clock::Now();
			secondsElapsed = 0;
			logger = std::make_shared<utils::loggers::ConsoleLogger>();
			Game::input.onQuit([this]() {
				running = false;
			});
			audio->initialize();
		}

		virtual void onGameStart() {};
		virtual void onGameEnd() {};
		virtual void onGameUpdate(double dt) {};
		virtual void onGamePhysicsUpdate(double dt) {};
		virtual void onGameBeforeUpdate() {};
		virtual void onGameAfterUpdate() {};

		Ptr<Scene> addScene(String sceneName) {
			Ptr<Scene> scene = scenes.add(sceneName);
			scene->name = sceneName;
			scene->game = this;
			return scene;
		}

		template <class T>
		Ptr<Scene> addScene(String sceneName) {
			Ptr<Scene> scene = scenes.add<T>(sceneName);
			scene->name = sceneName;
			scene->game = this;
			return scene;
		}

		static void initializeSDL() {
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
				throw new std::exception("Failed to initialize SDL");
			}
		}

		void run();

		void log(std::string message) {
			logger->log(message);
		}
	};
}

#endif