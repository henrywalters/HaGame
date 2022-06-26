#version 330 core

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

out vec2 TexCoords;
out vec4 Color;
out int Mode;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int mode; // 0 = Diffuse, 1 = Specular, 2 = Normal

uniform vec4 color;

void main() {

	TexCoords = a_texCoord;
	Mode = mode;
	Color = color;
	gl_Position = view * projection * model * vec4(a_vertex, 1.0);
}