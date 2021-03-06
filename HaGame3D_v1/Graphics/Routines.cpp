#include "Routines.h"
#include <glm/gtx/string_cast.hpp>

void hagame::graphics::drawMesh(Mesh mesh, ShaderProgram* shader) {
	
}

void hagame::graphics::drawCube(GLuint vao, ShaderProgram* shader, hagame::math::Hypercube<3, float> cube, Color color, float rotation)
{
	shader->use();

	glm::mat4 model = gl::getModelMatrix(cube.pos, cube.getCenter(), cube.size, rotation);

	shader->setMat4("model", model);
	shader->setVec4("color", color.getVec4());

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void hagame::graphics::drawTexture(GLuint vao, ShaderProgram* shader, Texture* texture, hagame::math::Hypercube<3, float> cube, Color color, float rotation)
{
	shader->use();
	glm::mat4 model = gl::getModelMatrix(cube.pos, cube.size * 0.5f, cube.size, rotation);

	shader->setMat4("model", model);
	shader->setVec4("color", color.getVec4());

	glActiveTexture(GL_TEXTURE0);
	texture->bind();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_TEXTURE_2D);
}

void hagame::graphics::drawSprite(GLuint vao, ShaderProgram* shader, Sprite sprite)
{
	shader->use();
	glm::mat4 model = gl::getModelMatrix(sprite.getPosition(), sprite.getOrigin(), sprite.getSize(), sprite.rotation);

	shader->setMat4("model", model);
	shader ->setVec4("color", sprite.color.getVec4());

	glActiveTexture(GL_TEXTURE0);
	sprite.texture->bind();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_TEXTURE_2D);
}

void hagame::graphics::drawParticle(GLuint vao, ShaderProgram* shader, Particle particle)
{
	shader->use();
	shader->setVec2("offset", glVec2(particle.position));
	shader->setVec4("color", particle.color.getVec4());
	particle.texture->bind();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
