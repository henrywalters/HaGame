#include "Camera.h"

hagame::graphics::Camera::Camera(Vec3UInt32 _size): size(_size) {
	yRot = 0;
	xRot = 0;
	position = hagame::math::Vector<3, float>::Zero();
	direction = hagame::math::Vector < 3, float>({ 0, 0, 1 });
	fov = 45.0f;
	zNear = 0.1f;
	zFar = 100.0f;
}

glm::mat4 hagame::graphics::Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(fov), (float)size[0] / (float)size[1], zNear, zFar);
}

glm::mat4 hagame::graphics::Camera::getViewMatrix() {
	direction = Vec3({ cos(xRot), sin(yRot), sin(xRot) + sin(yRot) });
	auto target = position + direction;
	return glm::lookAt(
		glm::vec3(position[0], position[1], position[2]),
		glm::vec3(target[0], target[1], target[2]),
		glm::vec3(0, 1, 0)
	);
}

GLint hagame::graphics::Camera::getMVPUniformLocation(ShaderProgram program)
{
	return glGetUniformLocation(program.id, "MVP");
}
