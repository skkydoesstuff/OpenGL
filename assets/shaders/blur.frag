#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uTexture;
uniform vec2 uOutputSize;
uniform bool uHorizontal;

const float weights[5] = float[](0.227027, 0.194595, 0.121622, 0.054054, 0.016216);

void main() {
    vec2 texel = 1.0 / uOutputSize;
    vec3 result = texture(uTexture, vTexCoord).rgb * weights[0];

    vec2 dir = uHorizontal ? vec2(texel.x, 0.0) : vec2(0.0, texel.y);
    for (int i = 1; i < 5; i++) {
        result += texture(uTexture, vTexCoord + dir * i).rgb * weights[i];
        result += texture(uTexture, vTexCoord - dir * i).rgb * weights[i];
    }

    fragColor = vec4(result, 1.0);
}