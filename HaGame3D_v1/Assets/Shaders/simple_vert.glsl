varying in vec3 aVertexPos;
varying in vec3 aNormal;

uniform mat4 MVP;
varying out vec3 Normal;

void main() {
	gl_Position = MVP * vec4(aVertexPos.x, aVertexPos.y, aVertexPos.z, 1);
	Normal = aNormal;
}