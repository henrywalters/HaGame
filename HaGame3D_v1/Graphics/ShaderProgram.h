
#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include "Shader.h"
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace hagame {
	namespace graphics {
		class ShaderProgram {
		public:
			GLuint id;
			hagame::graphics::Shader vertexShader;
			hagame::graphics::Shader fragmentShader;

			ShaderProgram(hagame::graphics::Shader vertex, hagame::graphics::Shader fragment);

			std::string getProgramLog();

			GLuint getUniform(std::string var);
			GLint getAttrib(std::string var);
			void setUniformInt(std::string var, int val);
			void setUniformFloat(std::string var, float val);
			void setUniformVec3(std::string var, glm::vec3 vect);
			void setUniformMat4(std::string var, glm::mat4 mat);
		};
	}
}

#endif