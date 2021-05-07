#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
	
	mat4 mvp = projection * view * model;
	mat4 normalMatrix = mat4(mvp);
	normalMatrix = transpose(inverse(normalMatrix));

	gl_Position = mvp * vec4(vertex.xyz, 1.0);
	FragPos = vec3(model * vec4(vertex.xyz, 1.0));
	// Normal = normal;
	Normal = vec3(model * vec4(normal.xyz, 1.0));
}