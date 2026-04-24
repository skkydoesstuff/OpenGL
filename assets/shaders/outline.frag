#version 460 core
in  vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform sampler2D uNormal;
uniform sampler2D uDepth;        // NEW: depth buffer
uniform vec2      uOutputSize;
uniform float     uNormalThreshold;
uniform float     uDepthThreshold;   // NEW
uniform float     uEdgeStrength;
uniform vec4      uOutlineColor;     // NEW: configurable tint
uniform float uEdgeWidth;  // 1.0 = normal, 2.0 = double width, etc.

const float NORMAL_THRESHOLD = 0.15;
const float DEPTH_THRESHOLD  = 0.01;
const float EDGE_STRENGTH    = 1.0;

// Safe normalize — avoids NaN on zero-length vectors (e.g. empty texels).
vec3 readNormal(vec2 uv) {
    vec3 n = texture(uNormal, uv).rgb * 2.0 - 1.0;
    float len = length(n);
    return (len > 1e-4) ? n / len : vec3(0.0, 0.0, 1.0);
}

// Linearise an OpenGL depth value from [0,1] NDC.
float readDepth(vec2 uv) {
    return texture(uDepth, uv).r;
}

void main() {
    vec2  texel     = 1.0 / uOutputSize;
    float nThresh   = (uNormalThreshold >= 0.0) ? uNormalThreshold : NORMAL_THRESHOLD;
    float dThresh   = (uDepthThreshold  >= 0.0) ? uDepthThreshold  : DEPTH_THRESHOLD;
    float strength  = (uEdgeStrength     >  0.0) ? uEdgeStrength    : EDGE_STRENGTH;

    // ── Normal Sobel (3×3) ──────────────────────────────────────────────────
    vec2 offset = texel * uEdgeWidth;

    vec3 nTL = readNormal(vTexCoord + offset * vec2(-1,  1));
    vec3 nTC = readNormal(vTexCoord + offset * vec2( 0,  1));
    vec3 nTR = readNormal(vTexCoord + texel * vec2( 1,  1));
    vec3 nML = readNormal(vTexCoord + texel * vec2(-1,  0));
    vec3 nMR = readNormal(vTexCoord + texel * vec2( 1,  0));
    vec3 nBL = readNormal(vTexCoord + texel * vec2(-1, -1));
    vec3 nBC = readNormal(vTexCoord + texel * vec2( 0, -1));
    vec3 nBR = readNormal(vTexCoord + texel * vec2( 1, -1));

    // Proper Sobel: cardinal weight=2, diagonal weight=1
    vec3 dx = (nTR + 2.0*nMR + nBR) - (nTL + 2.0*nML + nBL);
    vec3 dy = (nBL + 2.0*nBC + nBR) - (nTL + 2.0*nTC + nTR);
    float normalEdge = sqrt(dot(dx, dx) + dot(dy, dy));

    // ── Depth Sobel (scalar, same kernel) ───────────────────────────────────
    float dTL = readDepth(vTexCoord + texel * vec2(-1,  1));
    float dTC = readDepth(vTexCoord + texel * vec2( 0,  1));
    float dTR = readDepth(vTexCoord + texel * vec2( 1,  1));
    float dML = readDepth(vTexCoord + texel * vec2(-1,  0));
    float dMR = readDepth(vTexCoord + texel * vec2( 1,  0));
    float dBL = readDepth(vTexCoord + texel * vec2(-1, -1));
    float dBC = readDepth(vTexCoord + texel * vec2( 0, -1));
    float dBR = readDepth(vTexCoord + texel * vec2( 1, -1));

    float ddx = (dTR + 2.0*dMR + dBR) - (dTL + 2.0*dML + dBL);
    float ddy = (dBL + 2.0*dBC + dBR) - (dTL + 2.0*dTC + dTR);
    float depthEdge = sqrt(ddx*ddx + ddy*ddy);

    // ── Combine passes ──────────────────────────────────────────────────────
    float nEdge = smoothstep(nThresh, nThresh * 2.5, normalEdge);
    float dEdge = smoothstep(dThresh, dThresh * 3.0, depthEdge);

    // Union — keep whichever pass fired stronger at each pixel
    float edge = max(nEdge, dEdge);
    edge = clamp(edge * strength, 0.0, 1.0);

    // ── Output ──────────────────────────────────────────────────────────────
    vec4  src     = texture(uTexture, vTexCoord);
    vec4  outline = (uOutlineColor.a > 0.0) ? uOutlineColor : vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = vec4(mix(src.rgb, outline.rgb, edge * outline.a), src.a);
}