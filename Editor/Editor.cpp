#define SDL_MAIN_HANDLED 
#include "Editor.h"

int main() {

	Mat4 mat = Mat4({
		-0.707107, -0.000000, 0.707107, -0.000000,
		0.408248, 0.816497, 0.408248, -0.000000,
		-0.577350, 0.577350, -0.577350, -25.980761,
		0.000000, 0.000000, 0.000000, 1.000000
		});
	Mat4 b = mat.inverted();

	Vec4 vec = Vec4({ 1, 2, 3, 4 });

	//std::cout << (mat * vec).toString() << std::endl;

	std::cout << b.toString() << std::endl;

	//std::cout << (b * mat).toString() << std::endl;

	hagame::Game::initializeSDL();
	auto window = hagame::graphics::Window(hagame::graphics::MonitorManager::GetMonitor(0).displayBounds.size - Vec2({ 100, 100 }), "HaGame Editor");
	window.create();
	HaGameEditor editor = HaGameEditor(&window);
	editor.run();

	return 0;
}