#version 330 core

uniform vec3 color;
out vec4 LFragment;


void main() {
	LFragment = vec4(color, 1.0);
}