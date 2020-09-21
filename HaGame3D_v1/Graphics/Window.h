#include "../Math/Vector.h"
#include <string>
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>

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

			SDL_Window* window;
			SDL_GLContext gl;

			Window(hagame::math::Vector<2, uint32_t> _size, std::string _title);
		};
	}
}

#endif