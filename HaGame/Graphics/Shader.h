#ifndef SHADER
#define SHADER

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include "../Utils/Aliases.h"

namespace hagame {
	namespace graphics {

		class Shader {

		private:

			GLuint m_id;
			GLuint m_type;

		public:

			Shader(GLuint type): m_type(type), m_id(glCreateShader(type)) {};

			~Shader() {
				glDeleteShader(m_id);
			}

			GLuint getId() {
				return m_id;
			}

			bool compile(String source, bool crashOnError = true);

			// Static Members
			static Ptr<Shader> LoadVertex(std::string source);
			static Ptr<Shader> LoadFragment(std::string source);

			// Regular members
			std::string getShaderLog();
			
		};
	}
}

#endif