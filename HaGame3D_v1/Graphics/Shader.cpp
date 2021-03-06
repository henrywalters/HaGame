#include "Shader.h"

hagame::graphics::Shader hagame::graphics::Shader::Load(GLuint type, std::string source) {
	Shader shader = Shader();
	const char* cStr = source.c_str();
	shader.id = glCreateShader(type);
	glShaderSource(shader.id, 1, &cStr, NULL);
	GLint shaderCompiled = GL_FALSE;
	glCompileShader(shader.id);
	glGetShaderiv(shader.id, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE) {
		std::cout << shader.getShaderLog() << std::endl;
		throw new std::exception("Failed to compile shader. Please refer to console for more details");
	}
	return shader;
}

hagame::graphics::Shader hagame::graphics::Shader::LoadVertex(std::string source) {
	return hagame::graphics::Shader::Load(GL_VERTEX_SHADER, source);
}

hagame::graphics::Shader hagame::graphics::Shader::LoadFragment(std::string source) {
	return hagame::graphics::Shader::Load(GL_FRAGMENT_SHADER, source);
}

std::string hagame::graphics::Shader::getShaderLog() {
	if (glIsShader(id)) {
		int logLength = 0;
		int maxLength = logLength;

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];

		glGetShaderInfoLog(id, maxLength, &logLength, infoLog);

		printf("%s\n", infoLog);

		return infoLog;
	}
	else {
		return "Not a shader";
	}
}