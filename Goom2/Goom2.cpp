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
	//auto screenSize = hagame::graphics::MonitorManager::
	auto window = hagame::graphics::Window::ForMonitor(hagame::graphics::MonitorManager::GetMonitor(1));
	window.create();
	Goom2 goom = Goom2(&window);

	goom.run();

	return 0;
}