#version 330 core
in vec4 ParticleColor;
in vec3 Normal;

out vec4 color;

void main() {
	color = vec4(Normal.xyz, 1.0);
}