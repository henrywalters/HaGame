#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "Window.h"
#include "Monitors.h"

void hagame::graphics::Window::initGL() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glewExperimental = GL_TRUE;

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

	GLenum error = GL_NO_ERROR;
	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	}
}

hagame::graphics::Window::Window()
{
	mode = WindowMode::FULLSCREEN;
	pos = Vec2::Zero();
	size = Vec2::Zero();
}

hagame::graphics::Window::Window(Vec2 _size)
{
	mode = WindowMode::BORDERLESS_CENTERED;
	size = _size;
	pos = Vec2::Zero();
}

hagame::graphics::Window::Window(Vec2 _pos, Vec2 _size)
{
	mode = WindowMode::BORDERLESS;
	size = _size;
	pos = _pos;
}

hagame::graphics::Window::Window(Vec2 _size, std::string _title)
{
	mode = WindowMode::BORDERED_CENTERED;
	size = _size;
	pos = Vec2::Zero();
	title = _title;
}

hagame::graphics::Window::Window(Vec2 _pos, Vec2 _size, std::string _title)
{
	mode = WindowMode::BORDERED;
	size = _size;
	pos = _pos;
	title = _title;
}

hagame::graphics::Window hagame::graphics::Window::ForMonitor(Monitor monitor)
{
	Rect bounds = MonitorManager::GetDisplayBounds(monitor.id);
	return Window(bounds.pos, bounds.size);
}

void hagame::graphics::Window::create() {
	
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	int x = 0, y = 0, w = 0, h = 0;

	switch (mode) {
	case WindowMode::BORDERED:
		x = pos[0], y = pos[1], w = size[0], h = size[1];
		break;
	case WindowMode::BORDERED_CENTERED:
		x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED, w = size[0], h = size[1];
		break;
	case WindowMode::BORDERLESS:
		flags |= SDL_WINDOW_BORDERLESS;
		x = pos[0], y = pos[1], w = size[0], h = size[1];
		break;
	case WindowMode::BORDERLESS_CENTERED:
		flags |= SDL_WINDOW_BORDERLESS;
		x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED, w = size[0], h = size[1];
		break;
	case WindowMode::FULLSCREEN:
		flags |= SDL_WINDOW_FULLSCREEN;
		break;
	}

	window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

	if (window == NULL) {
		std::cout << "Failed to create SDL Window: " << SDL_GetError() << "\n";
	}

	initGL();
}

void hagame::graphics::Window::destroy() {
	SDL_DestroyWindow(window);
}

void hagame::graphics::Window::clear() {
	glClearColor(clearColor.rgb.r, clearColor.rgb.g, clearColor.rgb.b, clearColor.rgb.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void hagame::graphics::Window::render() {
	SDL_GL_SwapWindow(window);
}

void hagame::graphics::Window::setTitle(String title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

void hagame::graphics::Window::setViewport(Rect viewport)
{
	_viewport = viewport;
	glViewport(viewport.pos[0], size[1] - viewport.size[1] - viewport.pos[1], viewport.size[0], viewport.size[1]);
}

bool  hagame::graphics::Window::inViewport(Vec2 point)
{
	return _viewport.contains(point);
}

#endif