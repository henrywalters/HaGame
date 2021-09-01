#version 330 core

in vec4 Color;
flat in int Index;
out vec4 LFragment;

void main() {

	LFragment = vec4(Color.xyz,1.0);
}