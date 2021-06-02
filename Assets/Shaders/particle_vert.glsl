#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aVel;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aCreated;

out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float start;
uniform float gravity;

vec3 calcOffset() {
	float t = aCreated - start;
	return vec3(0.0) + (aVel * t) + (0.5 * gravity * t * t);
}

void main() {
	float scale = 10.0f;
	ParticleColor = aColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}