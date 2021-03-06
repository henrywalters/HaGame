#version 330 core
in vec2 TexCoords;
out vec4 LFragment;

uniform sampler2D image;
uniform vec4 color;

void main() {
	LFragment = color * texture(image, TexCoords);
}