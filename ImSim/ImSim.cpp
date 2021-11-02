#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#include "./ImSim.h"

int main() {
	hagame::Game::initializeSDL();

	Vec3 test = Vec3({ 1, 2, 3 });
	Vec2 testB = test.xy();
	std::cout << test.toString() << "\n";
	std::cout << testB.toString() << "\n";

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(1);
	auto window = hagame::graphics::Window(
		monitor.displayBounds.pos + Vec2(50.0f),
		monitor.displayBounds.size - Vec2({ 100, 100 }), 
		"HaGame Demo"
	);

	window.create();

	ImSim imsim(&window);

	imsim.run();

	return 0;
}