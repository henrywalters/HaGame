#version 330 core

#include "lighting.glsl"

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 Color;

layout (binding=0) uniform sampler2D ourTexture;
layout (binding=1) uniform sampler2D specTexture;
layout (binding=2) uniform sampler2D opacTexture;
layout (binding=3) uniform sampler2D normalMap;

uniform vec3 viewPos;

uniform Material material;
uniform PointLight light;

void main()
{
    if(texture(ourTexture, TexCoord).a < 0.1)
        discard;
    
    float attentuation = calcAttenuation(length(light.position - FragPos), light.constant, light.linear, light.quadratic);
    // float attentuation = 1.0f;

    // ambient light
    // vec3 ambient = lightColor * material.ambient;
    vec3 ambient = Color.xyz * light.ambient * texture(ourTexture, TexCoord).xyz * attentuation;

    // diffuse light
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(-vec3(-1, -1, 1));
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(ourTexture, TexCoord).xyz) * attentuation;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
    vec3 specular = light.specular * (spec * texture(specTexture, TexCoord).xyz) * attentuation;

    vec3 result = ambient + diffuse + specular;

    FragColor = texture(ourTexture, TexCoord) - vec4(0, 0, 0, 0.85); //vec4(result, texture(ourTexture, TexCoord).a * Color.w);

    //FragColor = Color;
}