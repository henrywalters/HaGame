#version 330 core

#include "lighting.glsl"

in vec2 TexCoords;
in vec4 Color;
in vec4 LightPos;
in vec4 VertPos;
in vec2 ScreenSize;

in mat4 Perspective;

uniform float pixelsPerMeter;
uniform float constant;
uniform float linear;
uniform float quadratic;

layout (binding=0) uniform sampler2D Normal;

out vec4 LFragment;
out float dist;

void main() {
	
	mat4 invPers = inverse(Perspective);

	vec2 fragPos = (gl_FragCoord.xy / ScreenSize);
	vec4 normal = texture(Normal, fragPos);
	if (normal.x != 0.0) {
		// normal.x *= 2.0;
		// normal.x -= 1.0;
		// normal.x *= 2.0;
	}

	if (normal.z != 0.0) {
		//normal.z -= 1.0;
		//normal.z *= 1.0;
	}

	vec2 lightPosScreen = vec2(LightPos.x * ScreenSize.x, LightPos.y * ScreenSize.y);

	float aspectRatio = ScreenSize.x / ScreenSize.y;

	vec3 fragPosNorm = vec3(fragPos.x * aspectRatio, fragPos.y, 0.0);
	vec3 lightPosNorm = vec3(LightPos.x * aspectRatio, LightPos.y, 1.0);

	dist = distance(gl_FragCoord.xy, lightPosScreen) / pixelsPerMeter;
	vec2 lightDir = gl_FragCoord.xy - lightPosScreen;
	float diff = max(0.0, dot(vec3(normal.x, normal.y, normal.z), vec3(-lightDir.x, 1.0, -lightDir.y)));

	LFragment = diff * Color;

	// LFragment = vec4(normal.x, 0.0,  0.0, 1.0);

	LFragment = Color;

	LFragment.a = calcAttenuation(dist, constant, linear, quadratic);
	
}