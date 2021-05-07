#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Demo.h"

int main() {


	hagame::Game::initializeSDL();
	auto window = hagame::graphics::Window(hagame::graphics::MonitorManager::GetMonitor(0).displayBounds.size - Vec2({ 100, 100 }), "HaGame Demo");
	window.create();

	Demo demo = Demo(&window);

	demo.run();

	return 0;
}