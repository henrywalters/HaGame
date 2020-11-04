#version 150

varying vec2 Tex;
varying vec3 FragPos;
varying vec3 Normal;

varying out vec4 LFragment;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

uniform vec3 Color;
uniform vec3 LightColor;
uniform float SpecularStrength;
uniform float AmbientStrength;
uniform float Shininess;
uniform vec3 AmbientColor;
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform sampler2D uTex;

void main() {
	
	vec3 ambient = AmbientStrength * AmbientColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	vec3 viewDir = normalize(ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
	vec3 specular = SpecularStrength * spec * LightColor;

	vec3 objectColor = texture(uTex, Tex).xyz;
	vec3 result = (diffuse + ambient + specular) * objectColor;

	LFragment = vec4(result, 1.0);
}