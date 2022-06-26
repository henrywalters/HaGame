#version 330 core
layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in vec3 a_pos;
layout(location = 4) in vec2 a_size;
layout(location = 5) in vec4 a_color;

out vec4 ParticleColor;
out vec3 Normal;

// uniform mat4 model;
uniform vec3 pos;
uniform vec3 cameraUp;
uniform vec3 cameraRight;
uniform mat4 view;
uniform mat4 projection;

void main() {

	vec3 vertexPos = pos + a_pos + (cameraRight * a_vertex.x * a_size.x ) + (cameraUp * a_vertex.y * a_size.y);

	ParticleColor = a_color;
	Normal = a_normal;

	gl_Position = projection * view * vec4(vertexPos, 1.0);
}