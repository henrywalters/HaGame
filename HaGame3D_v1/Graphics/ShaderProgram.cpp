#include "ShaderProgram.h"

hagame::graphics::ShaderProgram::ShaderProgram(hagame::graphics::Shader vertex, hagame::graphics::Shader fragment): vertexShader(vertex), fragmentShader(fragment) {
	id = glCreateProgram();
	glAttachShader(id, vertexShader.id);
	glAttachShader(id, fragmentShader.id);
	glLinkProgram(id);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(id, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		getProgramLog();
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

void hagame::graphics::ShaderProgram::use()
{
	glUseProgram(id);
}

GLuint hagame::graphics::ShaderProgram::getUniform(std::string var)
{
	return glGetUniformLocation(id, var.c_str());
}

GLint hagame::graphics::ShaderProgram::getAttrib(std::string var) {
	std::cout << "Shader id: " << id << std::endl;
	return glGetAttribLocation(id, var.c_str());
}

void hagame::graphics::ShaderProgram::setInt(std::string var, int val)
{
	glUniform1i(getUniform(var), val);
}

void hagame::graphics::ShaderProgram::setFloat(std::string var, float val)
{
	glUniform1f(getUniform(var), val);
}

void hagame::graphics::ShaderProgram::setVec2(std::string var, glm::vec2 vec) {
	glUniform2fv(getUniform(var), 1, &vec[0]);
}

void hagame::graphics::ShaderProgram::setVec3(std::string var, glm::vec3 vec) {
	glUniform3fv(getUniform(var), 1, &vec[0]);
}

void hagame::graphics::ShaderProgram::setVec4(std::string var, glm::vec4 vec) {
	glUniform4fv(getUniform(var), 1, &vec[0]);
}

void hagame::graphics::ShaderProgram::setMat4(std::string var, glm::mat4 mat) {
	glUniformMatrix4fv(getUniform(var), 1, GL_FALSE, &mat[0][0]);
}