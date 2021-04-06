#define SDL_MAIN_HANDLED 
#include "Editor.h"

int main() {

	hagame::math::Matrix<3, 4, float> mat = hagame::math::Matrix<3, 4, float>({
		0, 2, 1, 4,
		1,1, 2, 6,
		2, 1, 1, 7
		});
	mat.reduce();
	std::cout << mat.toString() << std::endl;

	hagame::Game::initializeSDL();
	auto window = hagame::graphics::Window(hagame::graphics::MonitorManager::GetMonitor(0).displayBounds.size - Vec2({ 100, 100 }), "HaGame Editor");
	window.create();
	HaGameEditor editor = HaGameEditor(&window);
	editor.run();

	return 0;
}