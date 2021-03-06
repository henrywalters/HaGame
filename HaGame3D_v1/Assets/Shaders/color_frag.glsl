#version 330 core

uniform vec4 color;

out vec4 LFragment;

void main() {
	LFragment = color;
}