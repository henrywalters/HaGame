#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "Goom2.h"
#include "SDL_mixer.h"

struct TestEvent {
	String message;

	TestEvent(String _message) : message(_message) {}
};

int main() {
	hagame::Game::initializeSDL();

	// constexpr std::uint32_fast32_t HANDGUN_ID{ 1 << 0 };
	auto screenSize = hagame::graphics::MonitorManager::GetMonitor(1).displayBounds;
	auto window = hagame::graphics::Window(screenSize.pos + screenSize.size * 0.05, screenSize.size * 0.9, "Goom 2: back to hell, again");
	window.create();
	Goom2 goom = Goom2(&window);

	goom.run();

	return 0;
}