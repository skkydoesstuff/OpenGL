#version 460 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 32
uniform Light lights[MAX_LIGHTS];
uniform int uNumLights;

uniform sampler2D uDiffuseMap;
uniform sampler2D uSpecularMap;
uniform float uHasSpecularMap;
uniform sampler2D uOpacityMap;
uniform bool uHasCutout;
uniform sampler2D uNormalMap;
uniform bool uHasNormalMap;
uniform float uShininess;
uniform float uOpacity;
uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;
in vec2 vTexCoord;
in mat3 TBN;

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gNormal;

vec3 calcLight(Light light, vec3 N, vec3 viewDir, vec3 diffuseTex, vec3 specularTex) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant +
                               light.linear * distance +
                               light.quadratic * distance * distance);

    float diff = max(dot(N, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(N, halfwayDir), 0.0), uShininess);

    vec3 ambient  = light.ambient  * diffuseTex * attenuation;
    vec3 diffuse  = light.diffuse  * diff * diffuseTex * attenuation;
    vec3 specular = light.specular * spec * specularTex * attenuation;

    return ambient + diffuse + specular;
}

void main() {
    // Normal
    vec3 N;
    if (uHasNormalMap) {
        N = texture(uNormalMap, vTexCoord).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
    } else {
        N = normalize(normal);
    }

    vec3 viewDir = normalize(viewPos - fragPos);

    // Diffuse
    vec4 diffuseSample = texture(uDiffuseMap, vTexCoord);
    vec3 diffuseTex = diffuseSample.rgb;

    // Specular
    vec3 specularTex = vec3(1.0);
    if (uHasSpecularMap > 0.5)
        specularTex = texture(uSpecularMap, vTexCoord).rgb;

    // Opacity
    float alpha = uOpacity;
    if (uHasCutout) {
        alpha *= texture(uOpacityMap, vTexCoord).r;
        if (alpha < 0.1)
            discard;
    }

    // Lighting
    vec3 result = vec3(0.0);
    for (int i = 0; i < uNumLights; i++)
        result += calcLight(lights[i], N, viewDir, diffuseTex, specularTex);

    gColor = vec4(result, alpha);
    gNormal = vec4(N * 0.5 + 0.5, 1.0);
}