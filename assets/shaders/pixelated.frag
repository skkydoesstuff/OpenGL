#version 460 core
in  vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform sampler2D uNormal;
uniform sampler2D uDepth;
uniform vec2      uOutputSize;

uniform float uPixelSize;  // e.g. 4.0 = 4x4 pixel blocks

void main() {
    vec2 pixelated = floor(vTexCoord * uOutputSize / uPixelSize) * uPixelSize / uOutputSize;
    FragColor = texture(uTexture, pixelated);
}