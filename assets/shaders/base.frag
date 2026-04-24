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
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
#define MAX_LIGHTS 32
uniform Light lights[32]; 
uniform int uNumLights;
uniform Material material;
uniform vec3 viewPos;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gNormal;
vec3 calcLight(Light light, vec3 N, vec3 viewDir, vec3 diffuseTex, vec3 specularTex) {
    vec3 lightDir   = normalize(light.position - fragPos);
    float distance  = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    float diff      = max(dot(N, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec      = pow(max(dot(N, halfwayDir), 0.0), material.shininess);
    vec3 ambient  = light.ambient  *        diffuseTex * attenuation;
    vec3 diffuse  = light.diffuse  * diff * diffuseTex * attenuation;
    vec3 specular = light.specular * spec * specularTex * attenuation;
    return ambient + diffuse + specular;
}
void main() {
    vec3 N          = normalize(normal);
    vec3 viewDir    = normalize(viewPos - fragPos);
    vec3 diffuseTex  = texture(material.diffuse,  texCoord).rgb;
    vec3 specularTex = texture(material.specular, texCoord).rgb;
    vec3 result = vec3(0.0);
    for (int i = 0; i < uNumLights; i++)
        result += calcLight(lights[i], N, viewDir, diffuseTex, specularTex);
    gColor  = vec4(result, 1.0);
    gNormal = vec4(N * 0.5 + 0.5, 1.0);
}