#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./ParticleEditor.h"

int main() {

	hagame::Game::initializeSDL();

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(0);
	auto window = hagame::graphics::Window(
		Vec2(1400, 900),
		"SpaceHorror"
	);

	window.create();

	ParticleEditor editor(&window);

	editor.run();

	return 0;
}