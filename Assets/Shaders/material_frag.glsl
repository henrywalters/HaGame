#version 330 core

#include "lighting.glsl"

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Material material;
uniform PointLight light;

out vec4 LFragment;

void main() {

	float attentuation = calcAttenuation(length(light.position - FragPos), light.constant, light.linear, light.quadratic);

	vec3 ambient = light.ambient * material.ambient * attentuation;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.ambient * (diff * material.diffuse) * attentuation;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.ambient * (spec * material.specular);
	
	LFragment = vec4(specular + diffuse + ambient, 1.0);
}