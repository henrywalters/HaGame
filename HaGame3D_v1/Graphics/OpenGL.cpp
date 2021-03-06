#include "OpenGL.h"

glm::mat4 hagame::graphics::gl::getModelMatrix(Vec3 position, Vec3 origin, Vec3 scale, float rotation) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glVec3(position));

	model = glm::translate(model, glVec3(origin));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glVec3(origin * -1));

	model = glm::scale(model, glVec3(scale));

	return model;
}
