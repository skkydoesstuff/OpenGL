#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;             // now world-space
out vec2 vTexCoord;
out mat3 TBN;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));  // world space ✓
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    normal      = normalMatrix * aNormal;   // world-space, scale-safe

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N); // re-orthogonalize (Gram-Schmidt)
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    vTexCoord    = aTexCoord;
}