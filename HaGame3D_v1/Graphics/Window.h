#include "../Math/Vector.h"
#include <string>
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include "Color.h"
#include <iostream>

#ifndef WINDOW
#define WINDOW

namespace hagame {
	namespace graphics {
		class Window {
		private:
			void initGL();

		public:
			hagame::math::Vector<2, uint32_t> size;
			std::string title;
			hagame::graphics::RGB clearColor;

			SDL_Window* window;
			SDL_GLContext gl;

			Window(hagame::math::Vector<2, uint32_t> _size, std::string _title);

			void clear();
			void render();
		};
	}
}

#endif