#version 330 core
layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

out vec2 TexCoords;
out vec4 Color;

// uniform mat4 model;
uniform vec3 cameraUp;
uniform vec3 cameraRight;
uniform vec3 pos;
uniform vec2 size;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;

void main() {

	vec3 vertexPos = pos + (cameraRight * a_vertex.x * size.x) + (cameraUp * a_vertex.y * size.y);

	TexCoords = a_texCoord;

	Color = color;
	gl_Position = projection * view * vec4(vertexPos, 1.0);
}