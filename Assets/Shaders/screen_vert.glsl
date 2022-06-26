#version 330 core
layout(location = 0) in vec3 a_vertex;
layout(location = 2) in vec2 aTexCoords;
uniform int mode; 

out vec2 TexCoords;
out int Mode;

void main()
{
    gl_Position = vec4(a_vertex.x, -a_vertex.y, a_vertex.z, 1.0);
    TexCoords = aTexCoords;
    Mode = mode;
}