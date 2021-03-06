#version 330 core
layout (location = 0) in vec4 vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = model * view * projection * vec4(vertex.xyz, 1.0);
}