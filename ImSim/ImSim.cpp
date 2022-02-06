#define SDL_MAIN_HANDLED 
#define WITH_SDL2
#define NDEBUG
#include "./ImSim.h"

uint64_t hash(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
	size_t intSize = sizeof(unsigned int) * CHAR_BIT;
	return (uint64_t)a << (intSize * 0) |
		(uint64_t)b << (intSize * 1) |
		(uint64_t)c << (intSize * 2) |
		(uint64_t)d << (intSize * 3);
}

int main() {
	hagame::Game::initializeSDL();

	hagame::graphics::Color c = hagame::graphics::Color(1.0f, 0.0f, 0.0f, 1.0f);
	hagame::graphics::Color end = hagame::graphics::Color(1.0f, 1.0f, 0.0f, 1.0f);

	for (int i = 0; i <= 10; i++) {
		auto lerped = hagame::graphics::Color::Lerp(c, end, i * 0.1f);
		std::cout << hagame::graphics::Color::RGB2HSV(lerped).toString() << "\n";
	}

	auto monitor = hagame::graphics::MonitorManager::GetMonitor(0);
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