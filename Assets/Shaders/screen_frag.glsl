
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

flat in int Mode;

layout (binding=0) uniform sampler2D geometry;
layout (binding=1) uniform sampler2D light;
layout (binding=2) uniform sampler2D normal;
layout (binding=3) uniform sampler2D specular;
layout (binding=4) uniform sampler2D unlit;

void main()
{
    vec4 lightTex = texture(light, TexCoords);
    vec4 unlitTex = texture(unlit, TexCoords);
    vec4 geomTex = texture(geometry, TexCoords);
    vec4 specTex = texture(specular, TexCoords);
    vec4 normTex = texture(specular, TexCoords);

    if (Mode == 0) {
        FragColor = vec4(geomTex.xyz, 1.0f);
        FragColor.xyz += unlitTex.rgb;
    } else if (Mode == 1) {
        FragColor = vec4(lightTex.xyz, 1.0f);
        FragColor.xyz += unlitTex.rgb;
    } else if (Mode == 2) {
        FragColor = vec4(normTex.xyz, 1.0f);
        FragColor.xyz += unlitTex.rgb;
    } else if (Mode == 3) {
        FragColor = vec4(specTex.xyz, 1.0f);
        FragColor.xyz += unlitTex.rgb;
    } else if (Mode == 4) {
        FragColor = vec4(unlitTex.xyz, 1.0f);
    } else {
        vec3 color = lightTex.rgb * geomTex.rgb;
        color += unlitTex.rgb;
        FragColor = vec4(color.xyz, 1.0f);
    }


}