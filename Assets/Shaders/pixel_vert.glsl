#version 330 core
layout (location = 0) in vec2 a_vertex;
layout (location = 1) in vec4 a_color;
layout (location = 2) in int a_index;

uniform mat4 projection;

out vec4 Color;
out int Index;

void main() {
	Color = a_color;
	Index = a_index;
	gl_Position = projection * vec4(a_vertex.xy, 0.0, 1.0);
}