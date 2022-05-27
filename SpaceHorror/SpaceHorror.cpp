#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./SpaceHorror.h"
#include "./Common/Config.h"

const String GAME_TITLE = "Space Horror";
const String GAME_CONFIG = "Game";
const String LAUNCH_SETTINGS = "LaunchSettings";

int main() {

	hagame::Game::initializeSDL();

	auto config = Config();
	auto gameConfig = config.load(GAME_CONFIG);
	auto monitor = hagame::graphics::MonitorManager::GetMonitor(
		gameConfig->getValue<int>(LAUNCH_SETTINGS, "monitor")
	);

	hagame::graphics::Window window;

	if (gameConfig->getBool("LaunchSettings", "fullscreen")) {
		window.pos = monitor.displayBounds.pos;
		window.size = monitor.displayBounds.size;
		window.mode = WindowMode::FULLSCREEN;
	}
	else {
		gameConfig->getArray<float, 2>(LAUNCH_SETTINGS, "window_size", window.size.vector);
		window.pos = monitor.displayBounds.pos + monitor.displayBounds.size * 0.5 - window.size * 0.5;
		window.mode = WindowMode::BORDERED;
		window.title = GAME_TITLE;
	}

	window.create();

	SpaceHorror fuckMeUp(&window);

	fuckMeUp.run();


	return 0;
}