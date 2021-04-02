#include "Camera.h"

Mat4 hagame::graphics::Camera::getOrthographicMatrix() {
	return Mat4::Orthographic(projectionSpace);
}

Mat4 hagame::graphics::Camera::getPerspectiveMatrix() {
	return Mat4::Perspective(projectionSpace);
}

Mat4 hagame::graphics::Camera::getViewMatrix() {
	
	// Camera's frame of view is facing up in the y axis and forward in the z axis.
	Vec3 top = Vec3::Top();
	Vec3 face = Vec3::Face();

	Vec3 direction = rotation.rotatePoint(face);
	Vec3 target = position + direction;
	Vec3 up = rotation.rotatePoint(top);

	std::cout << position.toString() << std::endl;
	std::cout << target.toString() << std::endl;
	std::cout << up.toString() << std::endl;

	return Mat4::LookAt(
		Vec3({ position[0], position[1], position[2] }),
		Vec3({ target[0], target[1], target[2] }),
		Vec3::Top()
	);
	
}

GLint hagame::graphics::Camera::getMVPUniformLocation(ShaderProgram program)
{
	return glGetUniformLocation(program.id, "MVP");
}
