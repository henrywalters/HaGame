#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Demo.h"

int main() {

	bool testing = false;

	if (!testing) {
		hagame::Game::initializeSDL();
		auto window = hagame::graphics::Window(hagame::graphics::MonitorManager::GetMonitor(0).displayBounds.size - Vec2({ 100, 100 }), "HaGame Demo");
		window.create();

		Demo demo = Demo(&window);

		demo.run();
	}

	//calcCellsThatIntersectLine(Vec3(5.0f), Vec3({ 28.120258, 0.000000, 21.894592 }), Vec3({ 7.155539, 0.000000, 1.159720 }));

	return 0;
}