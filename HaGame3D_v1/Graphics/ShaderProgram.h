
#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <SDL.h>
#include <SDL_opengl.h>
#include "Shader.h"

namespace hagame {
	namespace graphics {
		class ShaderProgram {
			GLuint id;
			hagame::graphics::Shader vertexShader;
			hagame::graphics::Shader fragmentShader;

			std::string getProgramLog();
		};
	}
}

#endif