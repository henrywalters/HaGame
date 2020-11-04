#version 150

in vec3 aVertexPos;
in vec3 aNormal;
in vec2 aTex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

varying out vec3 FragPos;
varying out vec3 Normal;
varying out vec2 Tex;

void main() {
	gl_Position = Projection * View * Model * vec4(aVertexPos, 1.0);
	FragPos = vec3(Model * vec4(aVertexPos, 1.0));
	Normal = aNormal;
	Tex = aTex;
}