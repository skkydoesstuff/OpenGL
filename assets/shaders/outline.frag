#version 460 core

in  vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform sampler2D uNormal;
uniform sampler2D uDepth;
uniform vec2      uOutputSize;

uniform float uDepthThreshold;   // Start with 0.01 for linear depth
uniform float uNormalThreshold;  // Start with 0.2
uniform vec3  uOutlineColor;

uniform float uEdgeWidth = 1.0;

uniform float uNear;  // Camera near plane (e.g., 0.1)
uniform float uFar;   // Camera far plane (e.g., 100.0)

float linearizeDepth(float depth) {
    // Convert from non-linear depth buffer to linear
    float z = depth * 2.0 - 1.0;  // to NDC
    return (2.0 * uNear * uFar) / (uFar + uNear - z * (uFar - uNear));
}

float getLinearDepth(vec2 uv) {
    return linearizeDepth(texture(uDepth, uv).r);
}

vec3 getNormal(vec2 uv) {
    return normalize(texture(uNormal, uv).rgb * 2.0 - 1.0);
}

void main() {
    vec2 texel = 1.0 / uOutputSize;
    vec2 offset = texel * uEdgeWidth;
    vec2 uv = vTexCoord;
    
    // Simple depth edge detection
    float depthCenter = getLinearDepth(uv);
    float depthLeft = getLinearDepth(uv + vec2(-offset.x, 0.0));
    float depthRight = getLinearDepth(uv + vec2(offset.x, 0.0));
    float depthUp = getLinearDepth(uv + vec2(0.0, offset.y));
    float depthDown = getLinearDepth(uv + vec2(0.0, -offset.y));
    
    float depthGradientX = abs(depthRight - depthLeft);
    float depthGradientY = abs(depthUp - depthDown);
    float depthEdge = max(depthGradientX, depthGradientY);
    
    // Simple normal edge detection
    vec3 normalCenter = getNormal(uv);
    vec3 normalLeft = getNormal(uv + vec2(-offset.x, 0.0));
    vec3 normalRight = getNormal(uv + vec2(offset.x, 0.0));
    vec3 normalUp = getNormal(uv + vec2(0.0, offset.y));
    vec3 normalDown = getNormal(uv + vec2(0.0, -offset.y));
    
    float normalGradientX = length(normalRight - normalLeft);
    float normalGradientY = length(normalUp - normalDown);
    float normalEdge = max(normalGradientX, normalGradientY);
    
    // Combine with thresholds
    float edge = 0.0;
    if (depthEdge > uDepthThreshold || normalEdge > uNormalThreshold) {
        edge = 1.0;
    }
    
    vec3 scene = texture(uTexture, uv).rgb;
    vec3 finalColor = mix(scene, uOutlineColor, edge);
    
    FragColor = vec4(finalColor, 1.0);
}