#version 460 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform sampler2D uNormal;

uniform vec2 uOutputSize;
uniform float uNormalThreshold;
uniform float uEdgeStrength;

const float DEFAULT_NORMAL_THRESHOLD = 0.15;
const float DEFAULT_EDGE_STRENGTH = 1.0;

vec3 readNormal(vec2 uv) {
    return normalize(texture(uNormal, uv).rgb * 2.0 - 1.0);
}

void main() {
    vec2 texel = 1.0 / uOutputSize;

    float threshold = (uNormalThreshold > 0.0)
        ? uNormalThreshold
        : DEFAULT_NORMAL_THRESHOLD;

    float strength = (uEdgeStrength > 0.0)
        ? uEdgeStrength
        : DEFAULT_EDGE_STRENGTH;

    // -----------------------------
    // Sobel-style sampling
    // -----------------------------
    vec3 nTL = readNormal(vTexCoord + texel * vec2(-1,  1));
    vec3 nTR = readNormal(vTexCoord + texel * vec2( 1,  1));
    vec3 nBL = readNormal(vTexCoord + texel * vec2(-1, -1));
    vec3 nBR = readNormal(vTexCoord + texel * vec2( 1, -1));

    vec3 nL  = readNormal(vTexCoord + texel * vec2(-1, 0));
    vec3 nR  = readNormal(vTexCoord + texel * vec2( 1, 0));
    vec3 nT  = readNormal(vTexCoord + texel * vec2(0,  1));
    vec3 nB  = readNormal(vTexCoord + texel * vec2(0, -1));

    vec3 center = readNormal(vTexCoord);

    // -----------------------------
    // Sobel gradients
    // -----------------------------
    vec3 dx = (nTR + 2.0 * nR + nBR) - (nTL + 2.0 * nL + nBL);
    vec3 dy = (nBL + 2.0 * nB + nBR) - (nTL + 2.0 * nT + nTR);

    float edge = dot(dx, dx) + dot(dy, dy);
    edge = sqrt(edge);

    // -----------------------------
    // stabilization (important)
    // -----------------------------
    edge = smoothstep(threshold, threshold * 2.5, edge);
    edge *= strength;

    edge = clamp(edge, 0.0, 1.0);

    // -----------------------------
    // output
    // -----------------------------
    vec3 color = texture(uTexture, vTexCoord).rgb;
    vec3 outlineColor = vec3(0.0);

    FragColor = vec4(mix(color, outlineColor, edge), 1.0);
}