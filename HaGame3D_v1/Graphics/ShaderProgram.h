
#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include "Shader.h"

namespace hagame {
	namespace graphics {
		class ShaderProgram {
		public:
			GLuint id;
			hagame::graphics::Shader vertexShader;
			hagame::graphics::Shader fragmentShader;

			ShaderProgram(hagame::graphics::Shader vertex, hagame::graphics::Shader fragment);

			std::string getProgramLog();
		};
	}
}

#endif