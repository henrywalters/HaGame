
#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include "Shader.h"
#include "../Utils/Aliases.h"
#include "Material.h"

namespace hagame {
	namespace graphics {
		class ShaderProgram {
		public:
			GLuint id;
			hagame::graphics::Shader vertexShader;
			hagame::graphics::Shader fragmentShader;

			ShaderProgram(hagame::graphics::Shader vertex, hagame::graphics::Shader fragment);

			std::string getProgramLog();

			void use();

			GLuint getUniform(std::string var);
			GLint getAttrib(std::string var);
			void setInt(std::string var, int val);
			void setFloat(std::string var, float val);
			void setVec2(std::string var, Vec2 vect);
			void setVec3(std::string var, Vec3 vect);
			void setVec4(std::string var, Vec4 vect);
			void setMat4(std::string var, Mat4 mat);
			void setMaterial(std::string var, Material mat);

			void setMVP(Mat4 model, Mat4 view, Mat4 projection);
		};
	}
}

#endif