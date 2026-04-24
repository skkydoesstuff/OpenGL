#version 460 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec2 uOutputSize;

uniform float THICKNESS;
uniform float DARKNESS;
uniform float BRIGHTBOOST;

vec3 RGBtoYIQ(vec3 c) {
    return mat3(
        0.2989,  0.5959,  0.2115,
        0.5870, -0.2744, -0.5229,
        0.1140, -0.3216,  0.3114
    ) * c;
}

vec3 YIQtoRGB(vec3 c) {
    return mat3(
        1.0,  1.0,  1.0,
        0.9560, -0.2720, -1.1060,
        0.6210, -0.6474,  1.7046
    ) * c;
}

void main() {
    vec4 color = texture(uTexture, vTexCoord);

    // brightness boost
    vec3 yiq = RGBtoYIQ(color.rgb);
    yiq.r *= BRIGHTBOOST;
    color.rgb = clamp(YIQtoRGB(yiq), 0.0, 1.0);

    // scanlines (pixel-accurate)
    float spacing = max(1.0, floor(THICKNESS));
    float lineIndex = gl_FragCoord.y;

    if (mod(lineIndex, spacing) < 1.0) {
        color.rgb *= (1.0 - DARKNESS);
    }

    FragColor = color;
}