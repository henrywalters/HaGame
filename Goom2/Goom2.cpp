#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Goom2.h"

int main() {
	hagame::Game::initializeSDL();
	auto window = hagame::graphics::Window(Vec2({ 800, 800 }), "Goom2: Let's do this");
	window.create();
	Goom2 goom = Goom2(&window);

	goom.run();

	return 0;
}