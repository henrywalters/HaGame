
#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include "Shader.h"
#include "../Utils/Aliases.h"
#include "Material.h"
#include "Light.h"

namespace hagame {
	namespace graphics {
		class ShaderProgram {
		public:
			GLuint id;
			String name;
			Ptr<hagame::graphics::Shader> vertexShader;
			Ptr<hagame::graphics::Shader> fragmentShader;

			ShaderProgram(String _name, Ptr <hagame::graphics::Shader> vertex, Ptr <hagame::graphics::Shader> fragment);
			~ShaderProgram() {
				glDeleteProgram(id);
			}

			std::string getProgramLog();

			void link();

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
			void setLight(std::string var, Vec3 pos, Light light);
			void setLight(std::string var, DirLight light);
			void setLight(std::string var, Vec3 pos, PointLight light);

			void setMVP(Mat4 model, Mat4 view, Mat4 projection);
			void setVP(Mat4 view, Mat4 projection);
		};
	}
}

#endif