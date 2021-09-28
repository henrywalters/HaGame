#version 330 core
in vec2 TexCoords;
in vec4 Color;
out vec4 LFragment;

uniform sampler2D image;

void main() {
	LFragment = texture(image, TexCoords);
}