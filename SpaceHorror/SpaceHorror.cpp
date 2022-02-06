#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./SpaceHorror.h"

int main() {
	hagame::Game::initializeSDL();

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(0);
	auto window = hagame::graphics::Window(
		monitor.displayBounds.pos + Vec2(50.0f),
		monitor.displayBounds.size - Vec2({ 100, 100 }),
		"SpaceHorror"
	);

	window.create();

	SpaceHorror fuckMeUp(&window);

	fuckMeUp.run();

	return 0;
}