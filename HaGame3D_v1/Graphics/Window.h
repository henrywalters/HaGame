#ifndef WINDOW
#define WINDOW

#include "../Math/Vector.h"
#include "../Utils/Aliases.h"
#include "Monitors.h"
#include <string>
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include "Color.h"
#include <iostream>

namespace hagame {
	namespace graphics {

		enum class WindowMode {
			BORDERED,
			BORDERED_CENTERED,
			BORDERLESS,
			BORDERLESS_CENTERED,
			FULLSCREEN,
		};

		class Window {
		private:
			void initGL();

		public:
			WindowMode mode;
			Vec2 size;
			Vec2 pos;
			std::string title;
			hagame::graphics::Color clearColor = hagame::graphics::Color::black();

			SDL_Window* window;
			SDL_GLContext gl;

			// Create a fullscreen window
			Window();

			// Create a centered, borderless window
			Window(Vec2 _size);

			// Create a position, borderless window
			Window(Vec2 _pos, Vec2 _size);

			// Create a centered, bordered window
			Window(Vec2 _size, std::string _title);

			// Create a positioned, bordered window
			Window(Vec2 _pos, Vec2 _size, std::string _title);

			static Window ForMonitor(Monitor monitor);

			// Create (and open) the window
			void create();

			// Destroy (and close) the window
			void destroy();

			// Clear all contents of window
			void clear();

			// Render the display buffer to window
			void render();
		};
	}
}

#endif