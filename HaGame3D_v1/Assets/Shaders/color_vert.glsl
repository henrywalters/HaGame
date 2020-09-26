varying in vec3 aVertexPos;
varying in vec3 aNormal;
varying in vec2 aTex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	gl_Position = Projection * View * Model * vec4(aVertexPos, 1.0);
}