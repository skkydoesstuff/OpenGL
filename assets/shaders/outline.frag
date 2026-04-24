#version 460 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture; // color
uniform sampler2D uNormal;  // normals
uniform vec2 uOutputSize;
uniform float uNormalThreshold;
uniform float uEdgeStrength;

const float DEFAULT_NORMAL_THRESHOLD = 0.2;
const float DEFAULT_EDGE_STRENGTH = 1.0;

vec3 readNormal(vec2 uv) {
    return normalize(texture(uNormal, uv).rgb * 2.0 - 1.0);
}

void main() {
    vec2 texel = 1.0 / uOutputSize;

    float normalThreshold = uNormalThreshold > 0.0 ? uNormalThreshold : DEFAULT_NORMAL_THRESHOLD;
    float edgeStrength = uEdgeStrength > 0.0 ? uEdgeStrength : DEFAULT_EDGE_STRENGTH;

    vec3 centerNormal = readNormal(vTexCoord);
    float normalDiff = 0.0;

    vec2 offsets[4] = vec2[](
        vec2(1.0, 0.0),
        vec2(-1.0, 0.0),
        vec2(0.0, 1.0),
        vec2(0.0, -1.0)
    );

    for (int i = 0; i < 4; ++i) {
        vec2 sampleUV = vTexCoord + offsets[i] * texel;
        vec3 sampleNormal = readNormal(sampleUV);
        normalDiff += 1.0 - max(dot(centerNormal, sampleNormal), 0.0);
    }

    float normalEdge = smoothstep(0.0, normalThreshold, normalDiff);
    float edge = clamp(normalEdge * edgeStrength, 0.0, 1.0);

    vec3 color = texture(uTexture, vTexCoord).rgb;
    vec3 outlineColor = vec3(0.0);

    FragColor = vec4(mix(color, outlineColor, edge), 1.0);
}