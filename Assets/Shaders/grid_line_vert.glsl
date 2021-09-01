#version 330 core
layout (location = 0) in vec2 a_vertex;
layout (location = 1) in vec2 a_pos;

uniform vec4 color;
uniform mat4 model;
uniform mat4 projection;

out vec4 Color;

void main() {
	Color = color;

	gl_Position = projection * vec4(a_vertex + a_pos, 0.0, 1.0);
}