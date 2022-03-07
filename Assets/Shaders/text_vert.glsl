#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 5, 1.0);
    // gl_Position.z = 1.0; // Move all text to the back
    TexCoords = vertex.zw;
}  