#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./SpaceHorror.h"

int main() {

	std::cout << stringTrim("HelloWorld") << "\n";

	hagame::Game::initializeSDL();

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(0); 
	auto window = hagame::graphics::Window(
		Vec2(800, 800),
		"SpaceHorror"
	);

	window.create();

	SpaceHorror fuckMeUp(&window);

	fuckMeUp.run();

	return 0;
}