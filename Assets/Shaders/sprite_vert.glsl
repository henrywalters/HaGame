#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;

void main() {
	TexCoords = vertex.zw;
	Color = color;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}