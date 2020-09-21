#include "Window.h"

hagame::graphics::Window::Window(hagame::math::Vector<2, uint32_t> _size, std::string _title) : size(_size), title(_title)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size[0], size[1], SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	gl = SDL_GL_CreateContext(window);

	if (gl == NULL) {
		throw new std::exception("Failed to initialize OpenGL");
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK) {
		throw new std::exception("Failed to initialize GLEW");
	}

	glewGetErrorString(glewError);

	if (SDL_GL_SetSwapInterval(1) < 0) {
		throw new std::exception("Failed to set VSync");
	}

	initGL();
}

void hagame::graphics::Window::initGL() {

}