#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Demo.h"

int main() {


	hagame::Game::initializeSDL();
	auto window = hagame::graphics::Window(hagame::graphics::MonitorManager::GetMonitor(0).displayBounds.size - Vec2({ 100, 100 }), "HaGame Demo");
	window.create();

	Demo demo = Demo(&window);

	demo.run();

	Quat quat = Quat(0.2, PI / 2, 0.4);
	std::cout << quat.toString() << std::endl;
	Vec3 euler = quat.eulerAngles();
	std::cout << euler.toString() << std::endl;

	return 0;
}