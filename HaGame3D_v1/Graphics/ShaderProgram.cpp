#include "ShaderProgram.h"

hagame::graphics::ShaderProgram::ShaderProgram(hagame::graphics::Shader vertex, hagame::graphics::Shader fragment): vertexShader(vertex), fragmentShader(fragment) {
	id = glCreateProgram();
	glAttachShader(id, vertexShader.id);
	glAttachShader(id, fragmentShader.id);
	glLinkProgram(id);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(id, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		throw new std::exception("Failed to link program. Check console for details");
	}
}

std::string hagame::graphics::ShaderProgram::getProgramLog() {
	if (glIsProgram(id)) {
		int logLength = 0;
		int maxLength = logLength;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];

		glGetProgramInfoLog(id, maxLength, &logLength, infoLog);

		printf("%s\n", infoLog);

		return infoLog;
	}
	else {
		return "Is not program";
	}
}