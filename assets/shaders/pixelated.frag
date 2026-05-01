#version 460 core
in  vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec2      uOutputSize;

uniform float uPixelSize;

void main() {
    vec2 pixelated = floor(vTexCoord * uOutputSize / uPixelSize) * uPixelSize / uOutputSize;
    FragColor = texture(uTexture, pixelated);
}