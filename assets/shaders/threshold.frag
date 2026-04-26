#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uTexture;
uniform float uThreshold;

void main() {
    vec3 color = texture(uTexture, vTexCoord).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    fragColor = vec4(brightness > uThreshold ? color : vec3(0.0), 1.0);
}