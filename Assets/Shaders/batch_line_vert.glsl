#version 330 core
layout (location = 0) in vec3 a_vertex;
layout(location = 1) in mat4 a_transform;
layout(location = 5) in vec4 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;
// uniform vec4 color;

void main() {
	Color = a_color;
	gl_Position = projection * view * model * a_transform * vec4(a_vertex.xyz, 1.0) ;
}