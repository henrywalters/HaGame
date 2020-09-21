// HaGame3D_v1.cpp : Defines the entry point for the application.
//
#define SDL_MAIN_HANDLED // insert this
#include "HaGame3D_v1.h"
#include "SDL.h"

using Vec3 = hagame::math::Vector<3, double>;

int main()
{	
	hagame::graphics::Window window = hagame::graphics::Window(hagame::math::Vector<2, uint32_t>({ 400, 400 }), "DEMO");
	auto fs = hagame::utils::FileSystem("../../../HaGame3D_v1/Assets");

	auto vert = fs.readFile("Shaders/simple_vert.glsl");
	auto frag = fs.readFile("Shaders/simple_Frag.glsl");

	auto vShader = hagame::graphics::Shader::LoadVertex(vert);
	auto fShader = hagame::graphics::Shader::LoadFragment(frag);

	// std::cout << vert << std::endl;

	auto gamepad = new hagame::input::devices::Gamepad(0);
	bool running = true;
	while (running) {
		gamepad->pollDevice();
		if (gamepad->startPressed) {
			std::cout << "Start" << std::endl;
		}

		if (gamepad->homePressed) {
			running = false;
		}
		// std::cout << gamepad->lAxis.toString() << std::endl;
	}
	return 0;
}
