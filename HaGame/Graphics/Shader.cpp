#include "shader.h"

bool hagame::graphics::Shader::compile(String source, bool crashOnError)
{
	const char* cStr = source.c_str();
	glShaderSource(m_id, 1, &cStr, NULL);
	GLint shaderCompiled = GL_FALSE;
	glCompileShader(m_id);
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE) {
		std::cout << "######## SHADER SOURCE CODE #######\n";
		auto lines = stringSplit(source, '\n');
		for (int i = 0; i < lines.size(); i++) {
			std::cout << (i + 1) << ": " << lines[i] << "\n";
		}

		std::cout << "\n######## END SOURCE CODE ########\n";
		std::cout << getShaderLog() << std::endl;

		if (crashOnError) {
			throw new std::exception("Failed to compile shader. Please refer to console for more details");
		}

		return false;
	}
	return true;
}


Ptr<hagame::graphics::Shader> hagame::graphics::Shader::LoadVertex(std::string source) {
	auto shader = std::make_shared<Shader>(GL_VERTEX_SHADER);
	shader->compile(source);
	return shader;
}

Ptr<hagame::graphics::Shader> hagame::graphics::Shader::LoadFragment(std::string source) {
	auto shader = std::make_shared<Shader>(GL_FRAGMENT_SHADER);
	shader->compile(source);
	return shader;
}

std::string hagame::graphics::Shader::getShaderLog() {
	if (glIsShader(m_id)) {
		int logLength = 0;
		int maxLength = logLength;

		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];

		glGetShaderInfoLog(m_id, maxLength, &logLength, infoLog);

		return infoLog;
	}
	else {
		return "Not a shader";
	}
}