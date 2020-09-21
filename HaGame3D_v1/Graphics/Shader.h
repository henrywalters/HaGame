#ifndef SHADER
#define SHADER

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>

namespace hagame {
	namespace graphics {
		class Shader {
		public:
			GLuint id;

			// Static Members
			static Shader Load(GLuint type, std::string source);
			static Shader LoadVertex(std::string source);
			static Shader LoadFragment(std::string source);

			// Regular members
			std::string getShaderLog();
			
		};
	}
}

#endif