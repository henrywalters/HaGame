#include "Routines.h"
#include <glm/gtx/string_cast.hpp>

/*
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
*/


// Draw a line with a given shader. Assumes MVP matrix is already set.
void hagame::graphics::drawLine(Line line, Color color, ShaderProgram* shader)
{
	shader->use();
	shader->setVec4("color", color);
	line.getMesh()->draw(shader);
}

void hagame::graphics::drawLine(hagame::math::Line line, Color color, ShaderProgram* shader, float thickness)
{
	auto lineMesh = getLineCache();
	lineMesh->setP1(line.a);
	lineMesh->setP2(line.b);
	lineMesh->setThickness(thickness);

	shader->use();
	shader->setMat4("model", Mat4::Identity());
	shader->setVec4("color", color);
	lineMesh->getMesh()->draw();
}

void hagame::graphics::drawRect(Rect rect, Color color, ShaderProgram* shader)
{
	Vec3 pos = rect.pos.resize<3>();
	Vec3 size = rect.size.resize<3>();
	drawLine(hagame::math::Line(pos, pos + size.prod(Vec3::Right())), color, shader);
	drawLine(hagame::math::Line(pos, pos + size.prod(Vec3::Top())), color, shader);
	drawLine(hagame::math::Line(pos + size.prod(Vec3::Top()), pos + size), color, shader);
	drawLine(hagame::math::Line(pos + size.prod(Vec3::Right()), pos + size), color, shader);
}

// Draw a cube outline with a given shader. Assumes MVP matrix is already set.
void hagame::graphics::drawCubeOutline(Cube cube, Color color, ShaderProgram* shader)
{
	drawLine(hagame::math::Line(cube.pos, cube.pos + cube.size.prod(Vec3::Face())), color, shader);
	drawLine(hagame::math::Line(cube.pos, cube.pos + cube.size.prod(Vec3::Right())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Right()), cube.pos + cube.size.prod(Vec3::Right()) + cube.size.prod(Vec3::Face())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Face()), cube.pos + cube.size.prod(Vec3::Right()) + cube.size.prod(Vec3::Face())), color, shader);

	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Top()), cube.pos + cube.size.prod(Vec3::Face()) + cube.size.prod(Vec3::Top())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Top()), cube.pos + cube.size.prod(Vec3::Right()) + cube.size.prod(Vec3::Top())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Right()) + cube.size.prod(Vec3::Top()), cube.pos + cube.size.prod(Vec3::Right()) + cube.size.prod(Vec3::Face()) + cube.size.prod(Vec3::Top())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Face()) + cube.size.prod(Vec3::Top()), cube.pos + cube.size.prod(Vec3::Right()) + cube.size.prod(Vec3::Face()) + cube.size.prod(Vec3::Top())), color, shader);

	drawLine(hagame::math::Line(cube.pos, cube.pos + cube.size.prod(Vec3::Top())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Right()), cube.pos + cube.size.prod(Vec3::Top()) + cube.size.prod(Vec3::Right())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Face()), cube.pos + cube.size.prod(Vec3::Top()) + cube.size.prod(Vec3::Face())), color, shader);
	drawLine(hagame::math::Line(cube.pos + cube.size.prod(Vec3::Face()) + cube.size.prod(Vec3::Right()), cube.pos + cube.size.prod(Vec3::Top()) + cube.size.prod(Vec3::Face()) + cube.size.prod(Vec3::Right())), color, shader);
}

void hagame::graphics::drawSurface(hagame::math::Surface surface, Mat4 transform, Color color, ShaderProgram* shader, float thickness)
{
	for (auto tri : surface.triangles) {
		auto transformedTri = tri.transformed(transform);
		drawTriangle(transformedTri, color, shader, thickness);
	}
}

void hagame::graphics::drawSphereOutline(Vec3 position, float radius, Color color, ShaderProgram* shader, float thickness)
{
	drawCircle(position, Vec3::Top(), radius, color, shader, thickness);
	drawCircle(position, Vec3::Right(), radius, color, shader, thickness);
	drawCircle(position, Vec3::Face(), radius, color, shader, thickness);
}

void hagame::graphics::drawCupsuleOutline(Vec3 pointA, Vec3 pointB, float radius, Color color, ShaderProgram* shader, float thickness)
{
	drawSphereOutline(pointA, radius, color, shader, thickness);
	drawSphereOutline(pointB, radius, color, shader, thickness);

	Vec3 perpY, perpX;
	Vec3 delta = pointB - pointA;
	perpX = delta.normalized().orth();
	perpY = delta.cross(perpX);
	perpY.normalize();

	perpX *= radius;
	perpY *= radius;

	drawLine(hagame::math::Line(pointA + perpX, pointB + perpX), color, shader, thickness);
	drawLine(hagame::math::Line(pointA - perpX, pointB - perpX), color, shader, thickness);
	drawLine(hagame::math::Line(pointA + perpY, pointB + perpY), color, shader, thickness);
	drawLine(hagame::math::Line(pointA - perpY, pointB - perpY), color, shader, thickness);
}

void hagame::graphics::drawCircle(Vec3 position, Vec3 axis, float radius, Color color, ShaderProgram* shader, float thickness)
{

	auto rot = Mat4::Rotation(Vec3::Top().rotationBetween(axis.normalized()));

	shader->use();
	shader->setVec4("color", color);
	shader->setMat4("model", Mat4::Translation(position) * rot);
	auto circle = getCircleCache();
	circle->setRadius(radius);
	circle->setThickness(thickness);
	auto mesh = circle->getMesh();
	mesh->draw(shader);
}

void hagame::graphics::drawTriangle(hagame::math::Triangle tri, Color color, ShaderProgram* shader, float thickness)
{
	drawLine(hagame::math::Line(tri.a, tri.b), color, shader, thickness);
	drawLine(hagame::math::Line(tri.b, tri.c), color, shader, thickness);
	drawLine(hagame::math::Line(tri.c, tri.a), color, shader, thickness);
}

void hagame::graphics::drawCone(Vec3 position, Vec3 axis, float radius, float height, Color color, ShaderProgram* shader)
{
	auto rot = Mat4::Rotation(Vec3::Top().rotationBetween(axis.normalized()));

	shader->use();
	shader->setVec4("color", color);
	shader->setMat4("model", Mat4::Translation(position) * rot);
	auto cone = getConeCache();
	cone->setHeight(height);
	cone->setRadius(radius);
	auto mesh = cone->getMesh();
	mesh->draw(shader);
}

void hagame::graphics::drawCube(Vec3 position, Vec3 size, Color color, ShaderProgram* shader)
{
	shader->use();
	shader->setVec4("color", color);
	shader->setMat4("model", Mat4::Translation(position));
	auto cube = getCubeCache();
	cube->setSize(size);
	auto mesh = cube->getMesh();
	mesh->draw(shader);
}

void hagame::graphics::drawRay(hagame::math::Ray ray, Color color, ShaderProgram* shader) {
	drawLine(ray.toLine(), color, shader);
	drawCone(ray.origin + ray.direction, ray.direction, 0.05f, 0.1f, color, shader);
}

void hagame::graphics::drawRotationGizmo(Vec3 position, float radius, ShaderProgram* shader, int axis)
{
	Color selectedColor = Color(255, 255, 0);
	drawCircle(position, Vec3::Top(), radius, axis == 1 ? selectedColor : Color::red(), shader);
	drawCircle(position, Vec3::Right(), radius, axis == 0 ? selectedColor : Color::blue(), shader);
	drawCircle(position, Vec3::Face(), radius, axis == 2 ? selectedColor : Color::green(), shader);
}


void hagame::graphics::drawMovementGizmo(Vec3 position, float size, ShaderProgram* shader, int axis)
{
	Color selectedColor = Color(255, 255, 0);
	glDisable(GL_DEPTH_TEST);
	drawRay(hagame::math::Ray(position, Vec3::Right() * size), axis == 0 ? selectedColor : Color::blue(), shader);
	drawRay(hagame::math::Ray(position, Vec3::Top() * size), axis == 1 ? selectedColor : Color::red(), shader);
	drawRay(hagame::math::Ray(position, Vec3::Face() * size), axis == 2 ? selectedColor : Color::green(), shader);
	glEnable(GL_DEPTH_TEST);
}

void hagame::graphics::drawScaleGizmo(Vec3 position, float size, ShaderProgram* shader, int axis)
{
	Color selectedColor = Color(255, 255, 0);
	glDisable(GL_DEPTH_TEST);

	drawLine(hagame::math::Line(position, position + Vec3::Right() * size), axis == 0 ? selectedColor : Color::blue(), shader, 0.01f);
	drawCube(position + Vec3::Right() * size, Vec3(0.1f), axis == 0 ? selectedColor : Color::blue(), shader);

	drawLine(hagame::math::Line(position, position + Vec3::Top() * size), axis == 1 ? selectedColor : Color::red(), shader, 0.01f);
	drawCube(position + Vec3::Top() * size, Vec3(0.1f), axis == 1 ? selectedColor : Color::red(), shader);

	drawLine(hagame::math::Line(position, position + Vec3::Face() * size), axis == 2 ? selectedColor : Color::green(), shader, 0.01f);
	drawCube(position + Vec3::Face() * size, Vec3(0.1f), axis == 2 ? selectedColor : Color::green(), shader);

	glEnable(GL_DEPTH_TEST);
}

void hagame::graphics::drawText(ShaderProgram* shader, Font* font, String message, Color color, Vec3 pos, float maxLength)
{
	shader->use();
	shader->setVec4("textColor", color);
	textBuffer.draw(font, message, pos, maxLength);
}
