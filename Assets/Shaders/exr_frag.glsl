#version 330 core
in vec2 TexCoords;
in vec4 Color;
flat in int Mode;
out vec4 LFragment;

layout (binding=0) uniform sampler2D combined;
layout (binding=1) uniform sampler2D diffCol;
layout (binding=2) uniform sampler2D diffDir;
layout (binding=3) uniform sampler2D glossCol;
layout (binding=4) uniform sampler2D glossDir;
layout (binding=5) uniform sampler2D Normal;


void main() {
	vec4 diff = texture(diffCol, TexCoords);
	vec4 diffDir = texture(diffDir, TexCoords);
	vec4 gloss = texture(glossCol, TexCoords);
	vec4 glossDir = texture(glossDir, TexCoords);
	vec4 normal = texture(Normal, TexCoords);
	
	if (Mode == 0) {
		LFragment = diff + diffDir;
		if (LFragment.r == 0 && LFragment.g == 0 && LFragment.b == 0) {
			LFragment.a = 0;
		}
	} else if (Mode == 1) {
		LFragment = gloss + glossDir;
	} else if (Mode == 2) {
		LFragment = normal;
	}
}