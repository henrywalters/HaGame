#include "../HaGame/HaGame.h"
#include "FPS/FPS.hpp"

class Demo : public hagame::Game {
public:

	hagame::utils::Timer timer;
	hagame::math::Sample<double, 60> fpsSample;

	Demo(hagame::graphics::Window* window) : Game(window) {}

	void onGameStart() {
		window->setTitle("HaGame Demo");

		scenes.add<FPS>("fps");

		fpsSample.onFull = [this]() {
			window->setTitle("HaGame Demo | " + std::to_string(fpsSample.average()) + " (" + std::to_string(fpsSample.stddev()) + "mu)");
			fpsSample.clear();
		};

		std::cout << "HaGame started in " << std::to_string(timer.elapsed()) << " nanoseconds\n";
	}	

	void onGameUpdate(double dt) {
		fpsSample.insert(fps / 1000000);

		if (input.keyboardMouse.start) {
			running = false;
		}
	}

	void onGameEnd() {
		std::cout << "Thanks for playing!";
	}
};