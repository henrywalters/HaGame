#version 150

uniform vec3 Color;

varying out vec4 LFragment;

void main() {
	LFragment = vec4(Color, 1.0f);
}