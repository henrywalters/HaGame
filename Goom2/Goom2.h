#ifndef GOOM_2_H
#define GOOM_2_H

#include "../HaGame/HaGame.h"

#include "./Scenes/MainMenu.h"
#include "./Scenes/Sandbox.h"

#include "./Systems/ResourceSystem.h"

const String MAIN_MENU = "main_menu";
const String SANDBOX = "sandbox";

class Goom2 : public hagame::Game {
public:

	hagame::utils::Timer timer;
	hagame::math::Sample<double, 60> fpsSample;

	Goom2(hagame::graphics::Window* window) : Game(window) {}

	void onGameStart() {

		resources->setBasePath("../../../Assets");

		fpsSample.onFull = [this]() {
			window->setTitle("Goom2: Back to hell, again | " + std::to_string(fpsSample.average()) + " (" + std::to_string(fpsSample.stddev()) + "mu)");
			fpsSample.clear();
		};

		// Add scenes

		addScene<MainMenu>(MAIN_MENU);
		addScene<Sandbox>(SANDBOX);

		scenes.forEach([this](Ptr<hagame::Scene> scene) {
			scene->addSystem<ResourceSystem>();
			scene->addSystem<hagame::graphics::RenderSystem>();
		});

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