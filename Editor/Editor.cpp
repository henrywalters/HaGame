#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Editor.h"

int main() {


	hagame::Game::initializeSDL();
	auto window = hagame::graphics::Window(hagame::graphics::MonitorManager::GetMonitor(0).displayBounds.size - Vec2({ 100, 100 }), "HaGame Editor");

	window.create();
	HaGameEditor editor = HaGameEditor(&window);
	editor.run();

	return 0;
}