#version 330 core
layout (location = 0) in vec3 a_vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;
uniform vec4 color;

void main() {
	Color = color;
	gl_Position = projection * view * model * vec4(a_vertex.xyz, 1.0);
}