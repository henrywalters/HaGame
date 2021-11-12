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
#include "Routines.h"

namespace hagame {
	namespace graphics {

		void GLAPIENTRY 
		glErrorHandler(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
		{
			
			 fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
				type, severity, message);
		}

		enum class WindowMode {
			BORDERED,
			BORDERED_CENTERED,
			BORDERLESS,
			BORDERLESS_CENTERED,
			FULLSCREEN,
		};

		enum class WindowRenderMode {
			Wireframe,
			Filled,
		};

		class Window {
		private:
			void initGLAttribs();
			void initGLContext();
			Rect _viewport;
			WindowRenderMode renderMode;
			

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

			// Only render wireframes of all objects
			void turnOnWireframeMode();

			// Render everything normally
			void turnOnFilledMode();

			// Toggle between wireframe and filled
			void toggleRenderMode();

			void setTitle(String title);

			void setViewport(Rect viewport);
			Rect getViewport() {
				return _viewport;
			}

			bool inViewport(Vec2 point);
		};
	}
}

#endif