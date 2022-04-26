#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./SpaceHorror.h"
#include "./../HaGame/Utils/Promise.h"
#include "./../HaGame/Utils/TimedCallback.h"

int main() {

	hagame::Game::initializeSDL();

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(0); 
	auto window = hagame::graphics::Window(
		Vec2(1400, 900),
		"SpaceHorror"
	);

	window.create();

	SpaceHorror fuckMeUp(&window);

	fuckMeUp.run();
	

	return 0;
}