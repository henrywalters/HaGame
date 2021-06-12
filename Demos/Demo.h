#include "../HaGame/HaGame.h"
#include "FPS/FPS.hpp"
#include "MiniJam/OneRoom.hpp"

class Demo : public hagame::Game {
public:

	hagame::utils::Timer timer;
	hagame::math::Sample<double, 60> fpsSample;

	Demo(hagame::graphics::Window* window) : Game(window) {
		
	}

	void onGameStart() {

		window->setTitle("HaGame Demo");

		addScene<FPS>("fps");
		addScene<MiniJamOneRoom>("minijam");

		//scenes.setActive("fps");
		scenes.setActive("minijam");

		resources->setBasePath("../../../Assets");

		fpsSample.onFull = [this]() {
			window->setTitle("HaGame Demo | " + std::to_string(fpsSample.average()) + " (" + std::to_string(fpsSample.stddev()) + "mu)");
			fpsSample.clear();
		};

		std::cout << "HaGame started in " << std::to_string(timer.elapsed()) << " nanoseconds\n";
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