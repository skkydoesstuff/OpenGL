#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uTexture;  // blurred result (ping-pong input, not used)
uniform sampler2D uScene;    // original scene
uniform sampler2D uBloom;    // blurred bright regions
uniform float uIntensity;

void main() {
    vec3 scene = texture(uScene, vTexCoord).rgb;
    vec3 bloom = texture(uBloom, vTexCoord).rgb;

    vec3 result = scene + bloom * uIntensity;
    result = result / (result + vec3(1.0));

    fragColor = vec4(result, 1.0);
}