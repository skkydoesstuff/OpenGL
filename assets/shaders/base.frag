#version 460 core

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Light light;  
uniform Material material;

uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gNormal;

void main() {
    vec3 N = normalize(normal);   // world-space unit normal

    // lighting (unchanged — just use N instead of norm)
    vec3 lightDir    = normalize(light.position - fragPos);
    float diff       = max(dot(N, lightDir), 0.0);
    vec3 viewDir     = normalize(viewPos - fragPos);
    vec3 halfwayDir  = normalize(lightDir + viewDir);
    float spec       = pow(max(dot(N, halfwayDir), 0.0), material.shininess);

    vec3 diffuseTex  = texture(material.diffuse,  texCoord).rgb;
    vec3 specularTex = texture(material.specular, texCoord).rgb;

    vec3 result = light.ambient  *        diffuseTex
                + light.diffuse  * diff * diffuseTex
                + light.specular * spec * specularTex;

    gColor  = vec4(result, 1.0);
    gNormal = vec4(N * 0.5 + 0.5, 1.0);   // world-space, encoded to [0,1]
}