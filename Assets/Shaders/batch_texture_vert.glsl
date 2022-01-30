#version 330 core

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in mat4 a_transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

out vec4 Color;
uniform vec4 color;

void main() {
	gl_Position = projection * view * model * a_transform * vec4(a_vertex, 1.0);
	Normal = a_normal;
	FragPos = vec3(model * a_transform * vec4(a_vertex, 1.0));
	TexCoord = a_texCoord;
	Color = color;
}