#version 330 core

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

out vec2 TexCoords;
out vec4 Color;
out vec4 LightPos;
out vec4 VertPos;
out vec2 ScreenSize;

out mat4 Perspective;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 lightPos;
uniform vec4 color;
uniform vec2 screenSize;

void main() {

	TexCoords = a_texCoord;
	ScreenSize = screenSize;

	Color = color;
	Perspective = view * projection * model;
	gl_Position = Perspective * vec4(a_vertex, 1.0);
	LightPos = lightPos;
	VertPos = vec4(a_vertex.xyz, 1.0);
}