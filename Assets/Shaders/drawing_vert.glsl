#version 330 core
layout (location = 0) in vec2 a_vertex;

uniform mat4 projection;

out vec4 Color;
uniform vec4 color;

void main() {
	Color = color;
	gl_Position = projection * vec4(a_vertex.xy, 0.0, 1.0);
}