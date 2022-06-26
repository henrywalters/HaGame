#version 330 core
in vec2 TexCoords;
in vec4 Color;
out vec4 LFragment;

uniform sampler2D image;

void main() {
	// XZ normal vector to be consistent with EXRs
	LFragment = vec4(0.0, 1.0, 0.0, 1.0);
}