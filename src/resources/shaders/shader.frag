#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D u_Diffuse;
uniform vec4 color;

void main() {
    FragColor = texture(u_Diffuse, vUV) * color;
}