#ifndef GOOM_2_H
#define GOOM_2_H

#include "../HaGame/HaGame.h"

#include "./Scenes/MainMenu.h"
#include "./Scenes/Sandbox.h"
#include "./Scenes/MapBuilder.h"

#include "./Systems/ResourceSystem.h"
#include "./Systems/PlayerSystem.h"

const String MAIN_MENU = "main_menu";
const String SANDBOX = "sandbox";
const String MAP_BUILDER = "map_builder";

class Goom2 : public hagame::Game {
public:

	hagame::utils::Timer timer;
	hagame::math::Sample<double, 60> fpsSample;

	Goom2(hagame::graphics::Window* window) : Game(window) {

		resources->setBasePath("../../../Assets");

		addScene<MainMenu>(MAIN_MENU);
		addScene<Sandbox>(SANDBOX);
		// addScene<MapBuilder>(MAP_BUILDER);

		scenes.forEach([this](Ptr<hagame::Scene> scene) {
			scene->addSystem<ResourceSystem>();
			scene->addSystem<hagame::graphics::RenderSystem>();
			scene->addSystem<hagame::physics::CollisionSystem>();
		});
	}

	void onGameStart() {

		fpsSample.onFull = [this]() {
			window->setTitle("Goom2: Back to hell, again | " + std::to_string(fpsSample.average()) + " (" + std::to_string(fpsSample.stddev()) + "mu)");
			fpsSample.clear();
		};

		scenes.activate(SANDBOX);
	}

	void onGameUpdate(double dt) {
		fpsSample.insert(fps / 1000000);

		if (input.keyboardMouse.isKeyDown(SDLK_DELETE)) {
			running = false;
		}

		if (input.keyboardMouse.startPressed) {
			window->toggleRenderMode();
		}
	}

	void onGameEnd() {
		std::cout << "Thanks for playing!";
	}
	
};


#endif