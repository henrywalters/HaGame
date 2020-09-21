in vec3 VertexPos;

void main() {
	gl_Position = vec4(VertexPos, 1);
}